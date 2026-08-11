import os
from pathlib import Path
import time
import uuid
import wave

from flask import abort, send_from_directory
import numpy as np
import torch
from flask_sock import Sock
from redis import Redis
from rq import Queue
from silero_vad import load_silero_vad
from flask_cors import CORS

from app import create_app

model = load_silero_vad()

flask_app = create_app()
sock = Sock(flask_app)
CORS(flask_app)


##added this to expose storage folders and add them to frontend

AUDIO_FILES = os.path.join(flask_app.root_path, 'storage','audio_files')
@flask_app.route('/audio/<path:filename>')
def serve_audio(filename):
    try:
        return send_from_directory(AUDIO_FILES, filename)
    except FileNotFoundError:
        abort(404)

TRANSCRIPTION = os.path.join(flask_app.root_path, 'storage','transcription')
@flask_app.route('/transcription/<path:filename>')
def serve_transcription(filename):
    try:
        return send_from_directory(TRANSCRIPTION, filename)
    except FileNotFoundError:
        abort(404)


redis_url = os.getenv("REDIS_URL", "redis://redis:6379/0")
redis_conn = Redis.from_url(redis_url)
task_queue = Queue(connection=redis_conn)

SILENCE_LIMIT = 8

def process_bytes(input_bytes):
    audio_int16 = np.frombuffer(input_bytes, dtype=np.int16)
    audio_float32 = audio_int16.astype(np.float32) / 32768.0
    tensor = torch.from_numpy(audio_float32)
    return tensor.unsqueeze(0)


def isSpeech(chunk, sampling_rate):
    tensor = process_bytes(chunk)
    return model(tensor, sampling_rate).item()

@sock.route('/receive_data')
def websocket_route(ws):
    channels = 1
    sample_width = 2
    frame_rate = 16000

    base_dir = Path(__file__).resolve().parent.parent
    storage_dir = base_dir / "back" / "app" / "storage" / "audio_files"
    storage_dir.mkdir(parents=True, exist_ok=True)

    output_path = storage_dir / f"{uuid.uuid4()}.wav"
    total_bytes = 0
    
    # Track state correctly
    has_spoken = False
    start_time = time.time()
    last_speech_time = time.time()
    file_initialized = False

    # Reset Silero's internal memory state for a fresh recording stream
    model.reset_states() 

    ws.timeout = 1.0

    try:
        print("Waiting for actual voice data from ESP32...")
        while True:
            try:
                data = ws.receive(timeout=3)
            except Exception:
                data = None

            current_time = time.time()

            # Handle timeout/disconnects
            if data is None or isinstance(data, str) or len(data) == 0:
                if (current_time - start_time) >= 10:
                    print("Timeout: No data received for 10 seconds.")
                    break
                continue

            if not file_initialized:
                wav_file = wave.open(str(output_path), "wb")
                wav_file.setnchannels(channels)
                wav_file.setsampwidth(sample_width)
                wav_file.setframerate(frame_rate)
                file_initialized = True
                print("Audio stream detected, initializing file storage...")

            wav_file.writeframes(data)
            total_bytes += len(data)

            # Evaluate VAD
            speech_prob = isSpeech(data, frame_rate)

            if speech_prob >= 0.5:
                last_speech_time = current_time
                if not has_spoken:
                    print("Speech detected! Now tracking for silence.")
                    has_spoken = True
            else:
                # Only start counting silence if the user has actually started speaking
                if has_spoken and (current_time - last_speech_time) >= SILENCE_LIMIT:
                    print(f"Silence threshold ({SILENCE_LIMIT}s) reached after speech, telling ESP32 to stop.")
                    try:
                        ws.send("STOP")
                        time.sleep(0.1)
                    except Exception:
                        pass
                    break
                
                # Optional: Hard timeout if they hold the button in a silent room forever
                elif not has_spoken and (current_time - start_time) >= 15:
                    print("15 seconds of pure silence passed without speaking. Stopping.")
                    try:
                        ws.send("STOP")
                    except:
                        pass
                    break

    finally:
        if file_initialized:
            wav_file.close()

        # Discard tiny accidental recordings
        if total_bytes < 32000:
            try:
                output_path.unlink()
                print("Recording too short, file discarded.")
            except FileNotFoundError:
                pass
        else:
            job = task_queue.enqueue(
                "app.api.background_tasks.save_file", 
                args=(str(output_path),), 
                job_timeout=600
            )
            print(f"Valid audio enqueued: {job.id}")

if __name__ == "__main__":
    flask_app.run(host="0.0.0.0", port=5000, debug=True)