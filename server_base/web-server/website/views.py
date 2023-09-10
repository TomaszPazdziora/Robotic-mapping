from flask import Blueprint, request, render_template, flash
from .models import Measurement
from . import db
from .solar_tracking import get_angles, get_altitude, get_azimuth


views = Blueprint('views', __name__)    

@views.route('/', methods=["GET", "POST"])
def hello():
    measurements = db.session.query(Measurement).all()
    dates = set(m.date.date() for m in measurements)
    azimuth = get_azimuth()
    altitude = get_altitude()

    if request.method == "GET":
        return render_template("home.html", data=0, labels=0, azimuth=azimuth, altitude=altitude)
    
    if request.method == "POST":
        dt = request.form.get('date')

        for d in dates:
            if d.strftime('%Y-%m-%d') == dt:
                labels = [m.date.strftime('%H:%M') for m in measurements if m.date.strftime('%Y-%m-%d') == dt]
                data =   [m.data for m in measurements if m.date.strftime('%Y-%m-%d') == dt]

                flash(f'Poprawnie wczytano dane dla dnia: {d}', category='success')
                return render_template("home.html", data=data, labels=labels, azimuth=azimuth, altitude=altitude)
            
        flash(f'W dniu {dt} nie wykonano pomiarów', category='error')
        return render_template("home.html", data=0, labels=0, azimuth=azimuth, altitude=altitude)
        
    

@views.route('/angle', methods=["GET"])
def get_ang():
    if request.method == "GET":
        return (get_angles())
    


@views.route('/measurements', methods=["GET", "POST"])
def get_data():
    if request.method == 'GET':     
        meas = Measurement.query.all()
        dates = set(m.date.date() for m in meas)
        return render_template('measurements.html', meas=meas, dates=dates)
    
    if request.method == 'POST':
        data = request.get_data().decode('utf-8')
        new_meas = Measurement(data=data)
        db.session.add(new_meas)
        db.session.commit()
        return 'Data added'
    