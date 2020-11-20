import motors,time

speed_val = [0,0]

def updateMotors():
    motors.setSpeed("LEFT",speed_val[0])
    motors.setSpeed("RIGHT",speed_val[1])
"""
drive with left and right motors at speeds at left and right,
for t seconds. If t is negative, set motors and don't turn them
off
"""
def drive(left,right,t=-1):
    global speed_val

    speed_val = [left,right]
    updateMotors()

    if t < 0:
        return
    time.sleep(t)
    
    speed_val = [0,0]
    updateMotors()

"""
Rotates anticlockwise
"""
def rotate(speed,t=-1):
    drive(-speed,speed,t)

"""
get current speed as [left,right] list
"""
def speed():
    return speed_val
