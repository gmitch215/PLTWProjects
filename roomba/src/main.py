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

def arm(percent, direction=FORWARD):
    arm_motor.spin_for(direction, 700 * (percent / 100), velocity=35)

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
    wait(0.5, SECONDS)
    stop()
    close_claw()
    backward()
    wait(1.5, SECONDS)
    stop()

while (True):
    forward()
    if (dist.object_distance() < 250):
        stop()
        wait(2, SECONDS)
        grip()
        wait(1, SECONDS)
        if (random.getrandbits(1)):
            turn_left()
        else:
            turn_right()
        stop()
        wait(1, SECONDS)
        forward()
        wait(3, SECONDS)
        open_claw()
        backward()
        wait(1.5, SECONDS)
        stop()
        wait(0.5, SECONDS)
        forward()
        