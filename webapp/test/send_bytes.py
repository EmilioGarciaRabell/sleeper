
import wave
from websockets.sync.client import connect

def send_bytes():
    chunk_size = 1024
    
    uri = "ws://127.0.0.1:5001/receive_data"

    with connect(uri) as websocket:
        with open("vad_file.wav", "rb") as f:
            while True:
                chunk = f.read(chunk_size)
                if not chunk:
                    break                
                websocket.send(chunk)

if __name__ == "__main__":
    send_bytes()