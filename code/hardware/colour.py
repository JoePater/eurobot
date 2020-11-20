from bus import bus

def colours_init():
    bus.write_byte_data(colour_i2c_address, 0x00, 0b11)
    
def get_red():
    l = bus.read_byte_data(colour_i2c_address, 0x16)
    h = bus.read_byte_data(colour_i2c_address, 0x17)
    return (h << 8) + l

def get_green():
    l = bus.read_byte_data(colour_i2c_address, 0x18)
    h = bus.read_byte_data(colour_i2c_address, 0x19)
    return (h << 8) + l
    
def get_blue():
    l = bus.read_byte_data(colour_i2c_address, 0x20)
    h = bus.read_byte_data(colour_i2c_address, 0x21)
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
