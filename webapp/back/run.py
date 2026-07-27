
from pathlib import Path

from app import create_app
from flask_sock import Sock
import wave
import time
from pathlib import Path
import time
import uuid
from redis import Redis
from rq import Queue
import torch
import numpy as np
from silero_vad import load_silero_vad


model = load_silero_vad()


flask_app = create_app()
sock = Sock(flask_app)

redis_conn = Redis(host="localhost", port=6379, db=0)
task_queue = Queue(connection=redis_conn)


def process_bytes(input_bytes):
    audio_int32 = np.frombuffer(input_bytes, dtype=np.int32)
    audio_float32 = audio_int32.astype(np.float32) / 2147483648.0
    tensor = torch.from_numpy(audio_float32)    
    return tensor.unsqueeze(0)
    

def isSpeech(chunk,SAMPLING_RATE):
    tensor = process_bytes(chunk)
    speech_prob = model(tensor, SAMPLING_RATE).item()
    return speech_prob


@sock.route('/receive_data')
def websocket_route(ws):  
    channels = 1
    sample_width = 2
    frame_rate = 16000
    
    
    random_uuid = uuid.uuid4()

    unique_file_path = str(random_uuid) + ".wav"
    
    base_dir = Path(__file__).resolve().parent.parent
    storage_dir = base_dir / "back"   / "app"  / "storage"/ "audio_files"
    output_file_path = storage_dir / unique_file_path
    
    ws.timeout = 1.0
    
    ##check if variable hasn't changed for more than 10 seconds
    not_Speech_Time = time.time()
    
    try:
        with wave.open(str(output_file_path), "wb") as output_file:
            
            output_file.setnchannels(channels)
            output_file.setsampwidth(sample_width)
            output_file.setframerate(frame_rate)
            
            
            print("Receiving data ...........")
            while True:
                
                try:
                    
                    data = ws.receive(timeout=1)
                
                except Exception as e:
                    print("error here")
                    data = None
                    
                current_time = time.time()

                if not data:
                    print("DATA IS NULL")
                    if (current_time - not_Speech_Time) >= 10:
                        print("Timeout: No speech detected for over 10 seconds.")
                        
                        ws.close(message=1000, reason="No speech for 10 seconds")
                        break
                    continue
                
                output_file.writeframes(data) 
                speechProb = isSpeech(data,frame_rate) 
                
                if speechProb >= 0.8:
                    print("SPEECH DETECTED")
                    not_Speech_Time = time.time()
                
                else:
                    print("SPEECH NOT DETECTED") 
                    
                if (current_time - not_Speech_Time) >= 10:
                    ws.close(message="DATA NOT RECEIVED FOR MORE THAN 10 SECONDS")
    
    finally:
        
        print("started queue")
        job = task_queue.enqueue('app.api.background_tasks.save_file', str(output_file_path))
        print(job.is_finished)
    

if __name__ == '__main__':
    flask_app.run(host="0.0.0.0", port=5001, debug=True)
    sock.init_app(flask_app)
    
    
    