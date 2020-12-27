from bus import bus
import time

rev_speed = 50

class MotorBoard:
    def updateCtrls(self,motor):
        ctrl = (self.enables[motor - 1] << 7) + (self.directions[motor - 1] << 6) \
            + (self.shorted[motor - 1] << 5)

        bus.write_byte_data(self.addr,(motor - 1) * 2,ctrl)

    def __init__(self,addr,revolver=None):
        self.addr = addr
        self.enables = [0,0]
        self.directions = [0,0]
        self.shorted = [1,1]
        self.updateCtrls(1)
        self.updateCtrls(2)

        self.revNum = revolver
    
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

    def startRevolver(self,steps):
        speed = rev_speed

        if self.revNum is None or self.steps == 0:
            return

        if self.steps < 0:
            speed = -speed
            steps = -steps

        self.disable(self.revNum)

        bus.write_byte_data(self.addr,4,steps)

        self.setSpeed(self.revNum,speed)
        self.enable(self.revNum)

    def revolverIsDone(self):
        if self.revNum is None:
            return 0

        return bus.read_byte_data(self.addr,4) == 0

boards = [MotorBoard(0x41),MotorBoard(0x42),MotorBoard(0x43,revolver=1)]

#tuples of (board,motor)
#motor_dict = {"A":(boards[0],1),"B":(boards[0],2)}

motor_dict = {
    "LEFT":(boards[0],1),
    "RIGHT":(boards[0],2)
    "CLAMP":(boards[1],1),
    "ROTATE":(boards[1],2),
    "REVOLVER":(boards[2],1),
    "LIFT":(boards[2],2),
}

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
    tup = motor_dict["REVOLVER"]

    tup[0].turnRevolver(steps)

    while not tup[0].revolverIsDone():
        time.sleep(0.1)

        
