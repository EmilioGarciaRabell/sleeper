
from run import flask_app
import whisper
from pathlib import Path
from datetime import date
from app import db
from app.models import Note
import time
from pathlib import Path

model = whisper.load_model("medium")

def transcribe(file,insert_id):
    
    print("TRANSCRIBING FILEEEEEE")
    result = model.transcribe(file, language="es")
    output = result["text"]
    base_dir = Path(__file__).resolve().parent.parent
    storage_dir = base_dir / "storage" / "transcription"
    file_name = str(insert_id) + ".txt"
    output_file_path = storage_dir / file_name
    Path(output_file_path).parent.mkdir(parents=True, exist_ok=True)
    with open(output_file_path,"w") as output_file:
        output_file.write(output)
    data = db.get_or_404(Note,insert_id)
    data.transcription_file_location = str(output_file_path)
    data.status = 'completed'
    db.session.commit()
    
    print(f"TRANSCRIPT COMPLETED FOR : {insert_id}")

def save_file(file):
        print("startttt")
        with flask_app.app_context():
            note = Note(audio_file_location=file,date=date.today(),status="processing")
            db.session.add(note)
            db.session.commit()
            time.sleep(10)
            transcribe(file,note.id)
        print("endddd")
        # id = note.id
        # transcribe(file,id)