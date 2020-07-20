import random
import time
import threading
#import robot

x_lock = threading.Lock()
x_cond = threading.Condition(x_lock)
x = 0

def a():
    global x,x_lock
    while True:
        time.sleep(random.randint(0,10)/10)
        x_lock.acquire()
        x += 1
        x_cond.notify()
        x_lock.release()

def b():
    global x,x_lock
    while True:
        x_lock.acquire()
        x_cond.wait()
        if x > 0:
            print("Hooray!")
            x = 0
        x_lock.release()
            
t = threading.Thread(target=a)
t.start()
while True:
    x_lock.acquire()
    x +=1
    print("Thread 2: ",x)
    x_lock.release()
    time.sleep(0.001)

move_lock = threading.Lock()

def __setDrive(left,right):
    pass

def setDriveSync(left,right,t):
    move_lock.acquire()
    __setDrive(left,right)
    time.sleep(t)
    __setDrive(0,0)
    move_lock.release()

def setDriveAsync(left,right,t):
    thr = threading.Thread(target=setDriveSync,args=(left,right,t))
    thr.start()

def wait():
    move_lock.acquire()
    move_lock.release()

def isMoving():
    return move_lock.locked()
    
setDriveAsync(100,100,5)
prepareClaw()
wait()
    
