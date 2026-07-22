
from pathlib import Path

from app import create_app
from flask_sock import Sock
from wave import Wave_write
import wave
import time
from pathlib import Path



app = create_app()
sock = Sock(app)


@sock.route('/receive_data')
def websocket_route(ws):  
    channels = 1
    sample_width = 2
    frame_rate = 16000 
    num_frames = 46986
    base_dir = Path(__file__).resolve().parent.parent
    print(base_dir)
    storage_dir = base_dir / "back"   / "app"  / "storage"/ "audio_files"
    output_file_path = storage_dir / "wav_file_web_sock.wav"
    
    
    with wave.open(str(output_file_path), "wb") as output_file:
        output_file.setnchannels(channels)
        output_file.setsampwidth(sample_width)
        output_file.setframerate(frame_rate)
        output_file.setnframes(num_frames)
        
        print("Receiving data ...........")
        while True:
            data = ws.receive()
            if data is None:
                break
            output_file.writeframes(data)    
        

if __name__ == '__main__':
    app.run(debug=True)
    sock.init_app(app)
    
    
    