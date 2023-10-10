from flask import Blueprint, request, render_template, flash
from .models import Measurement
from . import db
import json

# STATES
IDLE   = 0
MANUAL    = 1
TRACE     = 2
SCANNING  = 3
SANDBOX   = 4

TRACE_CMD_SIZE = 10

state = 0
left_speed = 0
right_speed = 0
direction = 0
trace_commands = ''


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
    global trace_commands
    state = TRACE

    if request.method == "GET":
        return render_template("trace.html", state=state, left_speed=left_speed, right_speed=right_speed)
    
    elif request.method == "POST":

        if 'stop_btn' in request.form:
            left_speed = 0
            right_speed = 0
        elif 'set_default_btn' in request.form:
            left_speed = request.form['default_speed']
            right_speed = left_speed
        elif 'drive_forwards_btn' in request.form:
            drive_forwards = request.form['drive_forwards']
            trace_commands += ('forwards: ' + drive_forwards + ', ')
        elif 'turn_left_btn' in request.form:
            left_shift = request.form['left_shift']
            trace_commands += ('turn left: ' + left_shift + ', ')
        elif 'turn_right_btn' in request.form:
            right_shift = request.form['right_shift']
            trace_commands += ('turn right: ' + right_shift + ', ')
        elif 'reset_trace_btn' in request.form:
            trace_commands = ''

        if left_speed == '':
            left_speed = 0
        if right_speed == '':
            right_speed = 0
            
        return render_template("trace.html", state=state, left_speed=left_speed, right_speed=right_speed, trace=trace_commands)


# route for esp testing
@views.route('/sandbox', methods=["GET"])
def sandbox():
    global state
    state = SANDBOX
    return f"state: {state}"

##########################################
#               DATA ROUTES              #
##########################################


@views.route('/data', methods=["GET"])
def index():
    x =  {  "state":int(state), 
            "lspeed":int(left_speed),
            "rspeed":int(right_speed)}
    y = json.dumps(x)
    return y

@views.route('/trace_cmd_data', methods=["GET"])
def trace_cmd_data():
    out_str = ''
    # for'w'ards
    # turn 'l'eft
    # turn ri'g'ht
    for c in trace_commands:
        if c == 'w':
            out_str += 'f'
        elif c == 'l':
            out_str += 'l'
        elif c == 'g':
            out_str += 'r'
        elif c.isdigit():
            out_str += c
        elif c == ',':
            out_str += c
    return out_str
