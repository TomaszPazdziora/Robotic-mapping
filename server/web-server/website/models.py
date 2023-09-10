from . import db
from sqlalchemy.sql import func
import datetime

class Measurement(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    data = db.Column(db.String(1000))
    date = db.Column(db.DateTime, default=datetime.datetime.now)

    def __repr__(self):
        return f"Measurement('{self.date}', '{self.data}')"