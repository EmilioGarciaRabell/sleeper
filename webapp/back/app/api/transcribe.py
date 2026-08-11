
import os

from flask import send_from_directory

from app.models import Note
from app import db
from sqlalchemy import select

from app.api import bp

@bp.route('/allposts',methods=["GET"])
def get_all_posts():
    # result = Note.query.order_by(Note.date).all()
    stmt = select(Note).order_by(Note.date.desc())
    users = db.session.scalars(stmt).all()
    output = []
    for i in range(len(users)):
        output.append(users[i].to_dict())
    return output
    

@bp.route("/",methods=["POST"])
def hello_world():
    return "hello"


