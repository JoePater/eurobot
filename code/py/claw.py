import motors

is_up = False
is_right_way_up = True
is_grabbed = False

claw_clamp_time = 2
claw_clamp_off_power = 8

colour_i2c_address = 0x

#buttonPressed("LIFT_UP")

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

    while not buttonPressed("LIFT_DOWN"):
        pass
    
    motors.setSpeed("LIFT",0)
    is_up = False

def lift():
    global is_up

    if is_up:
        return
    
    motors.setDirection("LIFT",clockwise=True)
    motors.setSpeed("LIFT",25)

    while not buttonPressed("LIFT_UP"):
        pass
    
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

    while not butt

def colours_init():
    write_byte_data(colour_i2c_address, 0x00, 0b11)
    
def get_red():
    l = read_byte_data(colour_i2c_address, 0x16)
    h = read_byte_data(colour_i2c_address, 0x17)
    return (h << 8) + l

def get_green():
    l = read_byte_data(colour_i2c_address, 0x18)
    h = read_byte_data(colour_i2c_address, 0x19)
    return (h << 8) + l
    
def get_blue():
    l = read_byte_data(colour_i2c_address, 0x20)
    h = read_byte_data(colour_i2c_address, 0x21)
    return (h << 8) + l

    
def get_colour():
    blue = get_blue()
    green = get_green()
    red = get_red()
    if blue > green and blue > red:
        return "blue"
    if red > blue and red > green:
        return "red"
    return "green"
