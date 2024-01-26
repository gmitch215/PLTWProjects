from vex import *
import random

brain = Brain()
left_motor = Motor(...)
right_motor = Motor(...)
arm_motor = Motor(...)
claw_motor = Motor(...)

optical = Optical(...)
bumper = Bumper(...)
dist = Distance(...)

# Functions
def forward():
    left_motor.spin(FORWARD, 100)
    right_motor.spin(FORWARD, 100)

def backward():
    left_motor.spin(REVERSE, 100)
    right_motor.spin(REVERSE, 100)

def turn_left():
    left_motor.spin(FORWARD, 100)

def turn_right():
    right_motor.spin(FORWARD, 100)

def stop():
    left_motor.stop()
    right_motor.stop()

# Begin Project

while (True):
    forward()
    if (dist.object_distance() < 500):
        stop()
        wait(1, SECONDS)
        backward()
        wait(2, SECONDS)
        stop()
        wait(1, SECONDS)
        if (random.getrandbits(1) == 1):
            turn_left()
        else:
            turn_right()
        wait(1, SECONDS)
        stop()
