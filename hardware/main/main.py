from fastapi import FastAPI, Request, Header
import os
import struct
app = FastAPI()

# Directory to store incoming audio
UPLOAD_DIR = "audio_recordings"
os.makedirs(UPLOAD_DIR, exist_ok=True)

@app.get("/")
async def root():
    return {"message": "Audio upload server is running!"}

def get_wav_header(sample_rate, bits_per_sample, num_channels, data_size):
    # RIFF header structure
    header = struct.pack('<4sI4s4sIHHIIHH4sI',
        b'RIFF', 36 + data_size, b'WAVE', b'fmt ', 16, 1, num_channels,
        sample_rate, sample_rate * num_channels * bits_per_sample // 8,
        num_channels * bits_per_sample // 8, bits_per_sample, b'data', data_size
    )
    return header

@app.post("/upload-audio")
async def upload_audio(
    request: Request, 
    x_file_mode: str = Header(..., alias="X-File-Mode")
):
    body = await request.body()
    print(f"Received {len(body)} bytes. Mode: {x_file_mode}")
    
    if x_file_mode == "new":
        # 1. Write the 44-byte header to a brand new file
        # Note: 0 is used as the data_size here because we don't know the final size yet
        header = get_wav_header(8000, 32, 1, 0) 
        with open("audio_upload.wav", "wb") as f:
            f.write(header)
    
    # 2. Append the incoming audio data
    with open("audio_upload.wav", "ab") as f:
        f.write(body)
        
    return {"status": "success", "received_bytes": len(body)}