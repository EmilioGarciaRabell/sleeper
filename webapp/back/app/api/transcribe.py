
import os

import socketio

from app.api import bp
from flask import request,current_app
import whisper
from pathlib import Path
from datetime import date
from app import db
from app.models import Note





model = whisper.load_model("tiny")

def transcribe(file,insert_id):
    result = model.transcribe(file)
    output = result["text"]
    base_dir = Path(__file__).resolve().parent.parent
    storage_dir = base_dir / "storage" / "transcription"
    output_file_path = storage_dir / "new_file.txt"
    with open(output_file_path,"w") as output_file:
        output_file.write(output)
    data = db.get_or_404(Note,insert_id)
    data.transcription_file_location = str(output_file_path)
    data.status = 'completed'
    db.session.commit()
    print(f"Successfully committed transcript for Note: {insert_id}")

def save_file(file):
    if file:
        file_path = os.path.join(current_app.config['UPLOAD_FOLDER'], file.filename)
        file.save(file_path)
        note = Note(audio_file_location=file_path,date=date.today(),status="processing")
        db.session.add(note)
        db.session.commit()
        id = note.id
        transcribe(file_path,id)
        



@bp.route('/allposts',methods=["POST"])
def get_all_posts():
    pass

@bp.route("/",methods=["POST"])
def hello_world():
    return "hello"