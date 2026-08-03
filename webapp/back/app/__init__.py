from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from config import Config



db = SQLAlchemy()
 
def create_app():

    app = Flask(__name__)
    UPLOAD_FOLDER = 'app/storage/audio_files'
    ALLOWED_EXTENSIONS = {".wav"}
    app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
    
    ##api blueprint
    from app.api import bp as api_bp
    app.register_blueprint(api_bp, url_prefix='/api')
    
    app.config.from_object(Config)
    db.init_app(app)
    
    with app.app_context():
        db.create_all()
    
    ##register database
    
    return app
    
    