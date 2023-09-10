import random
from turtle import Turtle, Screen

class Measurement:
    distance = 0    # distance travelled by vehicle 
    angle_shift = 0 # 
    reading = 0     # distance reading from ultrasonic sensor

    def __init__(self, distance, angle_shift, reading):
        self.distance = distance
        self.angle_shift = angle_shift
        self.reading = reading


Measurments = [Measurement(5,random.randint(-4, 4), random.randint(37, 40)) for _ in range(100)]

screen = Screen()
screen.setup(width=1000, height=800)

dot_size = 3
is_running = True

robot = Turtle()
dots = Turtle()

dots.penup()
dots.hideturtle()

robot.goto(0,0)
dots.goto(0,0)

robot.speed(0)
dots.speed(0)

for m in Measurments:
    robot.left(m.angle_shift)
    robot.forward(m.distance)

    dots.goto(robot.xcor(), robot.ycor())
    dots.setheading(robot.heading())
    dots.left(90)
    dots.forward(m.reading)
    dots.dot(dot_size, "red")

ts = robot.getscreen()
ts.getcanvas().postscript(file="duck.eps")

screen.exitonclick()