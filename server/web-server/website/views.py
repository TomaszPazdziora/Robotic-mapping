from flask import Blueprint, request, render_template, flash
import json

# STATES
IDLE   = 0
MANUAL    = 1
TRACE     = 2
SCANNING  = 3
SANDBOX   = 4

TRACE_CMD_SIZE = 10

class Position:
    x = 0
    y = 0
    angle = 0.0
    
state = 0
left_speed = 0
right_speed = 0
direction = 0
is_ready_to_scan = 'not'
is_ready_for_trace = 'not'
is_scan_sent = 'not'
trace_commands = 'scan, forwards: 100, scan, forwards: 100, scan, forwards: 100, scan, forwards: 100, scan, scan,forwards: 100, scan,'
CurrentPosition = Position()

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
    global is_ready_for_trace
    state = TRACE


    if request.method == "GET":
        return render_template("trace.html", state=state, left_speed=left_speed, right_speed=right_speed)
    
    elif request.method == "POST":

        if 'stop_btn' in request.form:
            left_speed = 0
            right_speed = 0
        elif 'start_btn' in request.form:
            is_ready_for_trace = 'ready'
        elif 'set_default_btn' in request.form:
            left_speed = request.form['default_speed']
            left_speed_int = int(left_speed) + 5
            right_speed = str(left_speed_int)
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
        elif 'scan_btn' in request.form:
            trace_commands += ('scan, ')

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
        elif c == 'c':
            out_str += 's'
        elif c.isdigit():
            out_str += c
        elif c == ',':
            out_str += c
    return out_str

# route for esp testing
@views.route('/lidar_data', methods=["POST"])
def lidar_data():
    if request.method == 'POST':
        data = request.get_data().decode('utf-8')
        with open("lidar_data.txt", "a") as text_file:
            text_file.write(data)
        return 'Data added'
    

@views.route('/occupancy', methods=["POST"])
def occupancy_data():
    if request.method == 'POST':
        data = request.get_data().decode('utf-8')
        with open("occupancy.txt", "w") as text_file:
            text_file.write(data)
        return 'Data added'
    

@views.route('/ready_to_scan', methods=["GET", "POST"])
def is_lidar_ready():
    global is_ready_to_scan
    if request.method == 'POST':
        data = request.get_data()
        is_ready_to_scan = 'ready'
        print(is_ready_to_scan)
        return 'Lidar state actualized'
    if request.method == 'GET':
        buff = is_ready_to_scan
        is_ready_to_scan = 'not'
        return buff
    

@views.route('/scan_sent', methods=["GET", "POST"])
def scan_sent():
    global is_scan_sent
    if request.method == 'POST':
        data = request.get_data()
        is_scan_sent = 'ready'
        return 'Lidar state actualized'
    if request.method == 'GET':
        buff = is_scan_sent
        is_scan_sent = 'not'
        return buff
    

@views.route('/ready_for_trace', methods=["GET"])
def is_ready_trace():
    global is_ready_for_trace
    if request.method == 'GET':
        buff = is_ready_for_trace
        is_ready_for_trace = 'not'
        return buff


@views.route('/current_position', methods=["GET", "POST"])
def get_current_position():
    global CurrentPosition
    if request.method == 'POST':
        data = request.get_data().decode('utf-8')
        print(data)
        arr = []
        arr = data.splitlines()
        CurrentPosition.x = int(arr[0])
        CurrentPosition.y = int(arr[1])
        CurrentPosition.angle = float(arr[2])
        return 'Position updated'
    if request.method == 'GET':
        ret_str = ''
        ret_str += str(CurrentPosition.x)
        ret_str += '\n'
        ret_str += str(CurrentPosition.y)
        ret_str += '\n'
        ret_str += str(CurrentPosition.angle)
        ret_str += '\n'
        print(ret_str)
        return ret_str
    

@views.route('/lidar_plot', methods=["GET"])
def lidar_plt():
    plot()
    return "done"

