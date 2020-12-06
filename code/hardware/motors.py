from bus import bus
import time

class MotorBoard:
    def updateCtrls(self,motor):
        ctrl = (self.enables[motor - 1] << 7) + (self.directions[motor - 1] << 6) \
            + (self.shorted[motor - 1] << 5)

        bus.write_byte_data(self.addr,(motor - 1) * 2,ctrl)

    def __init__(self,addr):
        self.addr = addr
        self.enables = [0,0]
        self.directions = [0,0]
        self.shorted = [1,1]
        self.updateCtrls(1)
        self.updateCtrls(2)
    
    def enable(self,motor):
        self.enables[motor - 1] = 1
        self.updateCtrls(motor)

    def disable(self,motor):
        self.enables[motor - 1] = 0
        self.updateCtrls(motor)

    def setDirection(self,motor,clockwise=1):
        self.directions[motor - 1] = 1 if clockwise else 0
        self.updateCtrls(motor)
        
    def setSpeed(self,motor,speed):
        if speed < 0:
            #invert direction
            self.setDirection(motor,0)
            speed = -speed
        else:
            self.setDirection(motor,1)
            
        if speed > 100:
            speed = 100
    
            
        x = round(255 * speed / 100)

        bus.write_byte_data(self.addr,motor * 2 - 1,x)

boards = [MotorBoard(0x43)]

#tuples of (board,motor)
motor_dict = {"A":(boards[0],1),"B":(boards[0],2)}

"""
motor_dict = {"LIFT":(0,1),
              "ROTATE":(0,2),
              "CLAMP":(0,3),
              "REVOLVER":(0,4),
              "LEFT":(1,1),
              "RIGHT":(1,2)}
"""

#takes string of motor name
def enableMotor(motor):
    tup = motor_dict[motor]
    tup[0].enable(tup[1])

def disableMotor(motor):
    tup = motor_dict[motor]
    tup[0].disable(tup[1])

def setSpeed(motor,speed):
    tup = motor_dict[motor]
    tup[0].setSpeed(tup[1],speed)

def setDirection(motor,clockwise=True):
    tup = motor_dict[motor]
    tup[0].setDirection(tup[1],clockwise)

def turnMotor(motor,clockwise=True,speed=25,t=1):
    enableMotor(motor)
    setDirection(motor,clockwise)
    setSpeed(motor,speed)
    time.sleep(t)
    setSpeed(motor,0)

# turns the revolver passed 'steps' slots
# negative means opposite direction
def turnRevolver(steps):
    if steps == 0:
        return

    if steps < 0:
        setDirection("REVOLVER",False)
        steps = -steps
    else:
        setDirection("REVOLVER",True)

    setSpeed("REVOLVER",60)
    enableMotor("REVOLVER")
    bus.write_byte_data(board_addr[0],9,steps)
    while 1:
        s = bus.read_byte_data(board_addr[0],9)
        if s == 0:
            return
        time.sleep(0.1)
        
