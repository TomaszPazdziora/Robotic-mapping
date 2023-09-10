from flask import Flask
from flask_sqlalchemy import SQLAlchemy 

db = SQLAlchemy()

def create_app():
    app = Flask(__name__)
    app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///database.db'
    app.config['SECRET_KEY'] = 'Afew!@#fdsf543'
    app.config["DEBUG"] = True
    db.init_app(app)
    
    from .views import views

    app.register_blueprint(views, url_prefix='/')
    
    with app.app_context():
        db.create_all()

    return app
