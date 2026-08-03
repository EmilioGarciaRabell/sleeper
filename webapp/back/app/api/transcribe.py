
import os


from app.api import bp

@bp.route('/allposts',methods=["POST"])
def get_all_posts():
    pass

@bp.route("/",methods=["POST"])
def hello_world():
    return "hello"