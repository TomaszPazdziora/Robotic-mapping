from flask import Blueprint, request, render_template, flash
from .models import Measurement
from . import db
import json

# STATES
IDLE   = 0
MANUAL    = 1
TRACE     = 2
SCANNING  = 3

state = 0
left_speed = 0
right_speed = 0
direction = 0


views = Blueprint('views', __name__)    

@views.route('/', methods=["GET", "POST"])
def home():
    global state
    state = IDLE

    if request.method == "GET":
        return render_template("home.html", state=0)


@views.route('/manual', methods=["GET", "POST"])
def manual():
    global left_speed
    global right_speed
    global state
    state = MANUAL

    if request.method == "GET":
        return render_template("manual.html", state=1, left_speed=left_speed, right_speed=right_speed)  
    
    elif request.method == "POST":

        if 'stop_btn' in request.form:
            left_speed = 0
            right_speed = 0
        elif 'set_default_btn' in request.form:
            left_speed = request.form['default_speed']
            right_speed = left_speed
        elif 'set_left_btn' in request.form:
            left_speed = request.form['left_speed']
        elif 'set_right_btn' in request.form:
            right_speed = request.form['right_speed']

        if left_speed == '':
            left_speed = 0
        if right_speed == '':
            right_speed = 0

        return render_template("manual.html", state=state, left_speed=left_speed, right_speed=right_speed)


@views.route('/trace', methods=["GET", "POST"])
def trace():
    global left_speed
    global right_speed
    global state
    state = TRACE

    return render_template("trace.html", state=state, left_speed=left_speed, right_speed=right_speed)


@views.route('/data', methods=["GET"])
def index():
    x =  {  "state":int(state), 
            "lspeed":int(left_speed),
            "rspeed":int(right_speed)}
    y = json.dumps(x)
    return y
