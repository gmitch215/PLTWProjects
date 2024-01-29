from vex import *
import random

brain = Brain()
left_motor = Motor(Ports.PORT10, GearSetting.RATIO_18_1, False)
right_motor = Motor(Ports.PORT1, GearSetting.RATIO_18_1, True)
arm_motor = Motor(Ports.PORT8, GearSetting.RATIO_18_1, True)
claw_motor = Motor(Ports.PORT3, GearSetting.RATIO_18_1, False)
sight = Optical(Ports.PORT16)
dist = Distance(Ports.PORT9)
bumper_a = Bumper(brain.three_wire_port.a)
led_d = Led(brain.three_wire_port.d)
vision = Vision(Ports.PORT5)

# Functions
def forward():
    left_motor.spin(FORWARD, 100)
    right_motor.spin(FORWARD, 100)

def backward():
    left_motor.spin(REVERSE, 100)
    right_motor.spin(REVERSE, 100)

def turn_left():
    left_motor.spin(FORWARD, 75)

def turn_right():
    right_motor.spin(FORWARD, 75)

def stop():
    left_motor.stop()
    right_motor.stop()

def arm(percent, direction=FORWARD, velocity=35):
    arm_motor.spin_for(direction, 700 * (percent / 100), velocity)

def claw(percent, direction=FORWARD):
    claw_motor.spin_for(direction, 270 * (percent / 100), velocity=50)

def open_claw():
    claw(100)

def close_claw():
    claw(100, REVERSE)

# Begin Project

def grip():    
    open_claw()
    forward()
    wait(500)
    stop()
    close_claw()
    backward()
    wait(500)


def cycle():
    arm(90, FORWARD, 100)
    claw(30, REVERSE)

    wait(1, SECONDS)
    arm(90, REVERSE, 50)

    while (dist.object_distance(INCHES) > 2):
        forward()
    
    stop()
    wait(1, SECONDS)
    grip()
    turn_left()
    wait(2, SECONDS)
    stop()


while (True):
    cycle()