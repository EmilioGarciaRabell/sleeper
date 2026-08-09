
import os

from app.models import Note

from app.api import bp

@bp.route('/allposts',methods=["GET"])
def get_all_posts():
    result = Note.query.all()
    output = []
    for i in range(len(result)):
        output.append(result[i].to_dict())
    return output
    

@bp.route("/",methods=["POST"])
def hello_world():
    return "hello"