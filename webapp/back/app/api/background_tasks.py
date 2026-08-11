
import os

from run import flask_app
import whisper
from pathlib import Path
from datetime import date
from app import db
from app.models import Note
import time


model = whisper.load_model("medium")
def transcribe(file,insert_id): 
    print("TRANSCRIBING FILEEEEEE")
    result = model.transcribe(file, language="es")
    output = result["text"]
    base_dir = Path(__file__).resolve().parent.parent
    storage_dir = base_dir / "storage" / "transcription"
    file_name = str(insert_id) + ".txt"
    output_file_path = storage_dir / file_name
    with open(output_file_path,"w") as output_file:
        output_file.write(output)
    data = db.get_or_404(Note,insert_id)
    
    ##add file_name instead of full path
    data.transcription_file_location = file_name
    data.status = 'completed'
    db.session.commit()
    
    print(f"TRANSCRIPT COMPLETED FOR : {insert_id}")

def save_file(file_path):
        print("startttt")
        with flask_app.app_context():
            filename = os.path.basename(file_path) 
            ##add only filename instead of full path
            print(filename)
            note = Note(audio_file_location=filename,date=date.today(),status="processing")
            db.session.add(note)
            db.session.commit()
            time.sleep(10)
            transcribe(file_path,note.id)
        print("endddd")