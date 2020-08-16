from smbus2 import SMBus
import time

bus = SMBus(1)

board_enables = [[0,0,0,0]
                 [0,0,0,0]]

board_addr = [123,124]

motor_dict = {"LIFT":(0,1),
              "ROTATE":(0,2),
              "CLAMP":(0,3),
              "REVOLVER":(0,4),
              "LEFT":(1,1),
              "RIGHT":(1,2)}

def updateEnables(motor):
    m = motor_dict[motor]

    en = board_enables[m[0]]
    byte = 8*en[0] + 4*en[1] + 2*en[2] + en[3]

    bus.write_byte_data(board_addr[m[0]],0,byte)

def enableMotor(motor):
    m = motor_dict[motor]
    board_enables[m[0]][m[1]] = 1
    updateEnables(motor)

def disableMotor(motor):
    m = motor_dict[motor]
    board_enables[m[0]][m[1]] = 0
    updateEnables(motor)

def setSpeed(motor,speed):
    if speed > 100:
        speed = 100
    if speed < 0:
        speed = 0

    x = 255 * speed / 100

    m = motor_dict[motor]
    addr = board_addr[m[0]]
    reg = m[1] * 2

    bus.write_bus_data(addr,reg,x)

def setDirection(motor,clockwise=True):
    x = 1
    if not clockwise:
        x = 2

    m = motor_dict[motor]
    addr = board_addr[m[0]]
    reg = m[1] * 2 - 1

    bus.write_bus_data(addr,reg,x)

def turnMotor(motor,clockwise=True,speed=25,t=1):
    enableMotor(motor)
    setDirection(motor,clockwise)
    setSpeed(motor,speed)
    time.sleep(t)
    setSpeed(motor,0)

    
        