from vex import *

brain = Brain()
left_motor = Motor(Ports.PORT10, GearSetting.RATIO_18_1, False)
right_motor = Motor(Ports.PORT1, GearSetting.RATIO_18_1, True)
arm_motor = Motor(Ports.PORT8, GearSetting.RATIO_18_1, True)
claw_motor = Motor(Ports.PORT3, GearSetting.RATIO_18_1, False)
sight = Optical(Ports.PORT16)
dist = Distance(Ports.PORT9)
dist_back = Distance(Ports.PORT7)
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

def arm(percent, direction=FORWARD, velocity=35, wait=True):
    arm_motor.spin_for(direction, 100 * (percent / 100), DEGREES, velocity, VelocityUnits.PERCENT, wait)

def claw(percent, direction=FORWARD, velocity=50, wait=True):
    claw_motor.spin_for(direction, 270 * (percent / 100), DEGREES, velocity, VelocityUnits.PERCENT, wait)

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
    arm(100, FORWARD, 100, wait=False)
    claw(30, REVERSE, wait=False)

    wait(1, SECONDS)
    claw(30, FORWARD, wait=False)
    arm(90, REVERSE, 50, wait=False)

    while (dist.object_distance(INCHES) > 2):
        backward()
    
    stop()
    wait(1, SECONDS)
    grip()
    turn_left()
    wait(2, SECONDS)
    stop()

# while (True):
#     cycle()
    
arm_motor.spin(FORWARD, 100)