from typing import Optional
import sqlalchemy as sa
import sqlalchemy.orm as so
from app import db
from datetime import date
from sqlalchemy import Date,String

class Note(db.Model):
    __tablename__ = "audio_notes"
    id: so.Mapped[int] = so.mapped_column(primary_key=True)
    audio_file_location: so.Mapped[str] = so.mapped_column()
    transcription_file_location: so.Mapped[Optional[str]] = so.mapped_column()
    date: so.Mapped[date] = so.mapped_column(Date)
    status: so.Mapped[str] = so.mapped_column(String(100))

    def to_dict(self):
        data = {
            "id" : self.id,
            "audio_file_location":self.audio_file_location,
            "transcription_file_location":self.transcription_file_location,
            "date": self.date,
            "status": self.status
        }
        return data