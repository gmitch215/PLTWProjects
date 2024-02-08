from vex import *

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

forward()

while True:
    if (bumper_a.pressing()):
        led_d.on()
    else:
        led_d.off()
    
    hue = sight.hue()
    rgb = sight.color()

    if (hue < 10 or hue > 350): # type: ignore
        stop()
        wait(1, SECONDS)
        turn_left()
        wait(1, SECONDS)
        stop()
        forward()
    elif (rgb < 0x373737): # type: ignore
        stop()
        wait(1, SECONDS)
        turn_right()
        wait(1, SECONDS)
        stop()
        forward()
    else:
        forward()
