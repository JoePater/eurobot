import motors,time

speed_val = [0,0]

def updateMotors():
    setSpeed("LEFT",speed_val[0])
    setSpeed("RIGHT",speed_val[1])

def drive(left,right,t=-1):
    global speed_val

    speed_val = [left,right]
    updateMotors()

    if t < 0:
        return
    time.sleep(t)
    
    speed_val = [0,0]
    updateMotors()

def rotate(speed,t=-1):
    drive(-speed,speed,t)

def speed():
    return speed_val
