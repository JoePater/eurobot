import motors, buttons

is_up = False
is_right_way_up = True
is_grabbed = False

claw_clamp_time = 2
claw_clamp_off_power = 8

colour_i2c_address = 0x

def isUp():
    return is_up

def isRightWayUp():
    return is_right_way_up

def isGrabbed():
    return is_grabbed


def goDown():
    global is_up

    if not is_up:
        return
    
    motors.setDirection("LIFT",clockwise=False)
    motors.setSpeed("LIFT",25)

    while not buttons.buttonPressed("LIFT_DOWN"):
        time.sleep(0.05)
    
    motors.setSpeed("LIFT",0)
    is_up = False

def lift():
    global is_up

    if is_up:
        return
    
    motors.setDirection("LIFT",clockwise=True)
    motors.setSpeed("LIFT",25)

    while not buttons.buttonPressed("LIFT_UP"):
        time.sleep(0.05)
    
    motors.setSpeed("LIFT",0)
    is_up = True

def grab():
    global is_grabbed

    motors.setDirection("CLAMP",clockwise=False)
    motors.setSpeed("CLAMP",25)

    time.sleep(claw_clamp_time)

    motors.setSpeed("CLAMP",claw_clamp_off_power)
    is_grabbed = True

def release():
    global is_grabbed

    motors.setDirection("CLAMP",clockwise=True)
    motors.setSpeed("CLAMP",25)

    while not buttons.buttonPressed("CLAW_OPEN"):
        time.sleep(0.05)

    motors.setSpeed("CLAMP",0)
    is_grabbed = False

def turnRightWayUp():
    global is_right_way_up

    motors.setDirection("ROTATE",clockwise=False)
    motors.setSpeed("ROTATE",25)

    while not buttons.buttonPressed("RIGHT_WAY_UP"):
        time.sleep(0.05)

    motors.setSpeed("ROTATE",0)
    is_right_way_up = True

def turnUpsideDown():
    global is_right_way_up

    motors.setDirection("ROTATE",clockwise=True)
    motors.setSpeed("ROTATE",25)

    while not buttons.buttonPressed("UPSIDE_DOWN"):
        time.sleep(0.05)

    motors.setSpeed("ROTATE",0)
    is_right_way_up = False
    
