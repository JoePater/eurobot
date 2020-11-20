#!/bin/python

import mainWindow
from tkinter import *
import time

claw = ["DOWN","RIGHT_WAY_UP","OPEN"]
wheels = [0,0]

def lift_claw():
    global claw
    time.sleep(1)
    claw[0] = "UP"

def lower_claw():
    global claw
    time.sleep(1)
    claw[0] = "DOWN"

def claw_right_way():
    global claw
    time.sleep(1)
    claw[1] = "RIGHT_WAY_UP"

def claw_upside_down():
    global claw
    time.sleep(1)
    claw[1] = "UPSIDE_DOWN"

def grab_claw():
    global claw
    time.sleep(1)
    claw[2] = "CLOSED"

def open_claw():
    global claw
    time.sleep(1)
    claw[2] = "OPEN"

def set_left_wheel(x):
    global wheels
    wheels[0] = x

def set_right_wheel(x):
    global wheels
    wheels[1] = x

def set_wheels(x):
    set_left_wheel(x)
    set_right_wheel(x)

rgb = "RED"

def set_rgb(x):
    global rgb
    rgb = ["RED","GREEN","BLUE"][x]
    
def get_rgb():
    return rgb
    
robot_buttons = [
    ["Lift claw",lift_claw],
    ["Lower claw",lower_claw],
    ["Claw right way up",claw_right_way],
    ["Claw upside down",claw_upside_down],
    ["Open claw",open_claw],
    ["Grab claw",grab_claw],
]

robot_sliders = [
    ["Set left wheel",set_left_wheel,-100,100],
    ["Set right wheel",set_right_wheel,-100,100],
    ["Set both wheels",set_wheels,-100,100],
    ["RGB",set_rgb,0,2],
]

variable_functions = {
    "Colour sensor":get_rgb,
    "Claw":lambda: "{},{},{}".format(claw[0],claw[1],claw[2]),
    "Wheel speeds":lambda: wheels,
}

def main():
    root = Tk()
    m = mainWindow.MainWindow(root,robot_buttons,robot_sliders,
                              variable_functions,globals())
    m.pack()

    try:
        while 1:
            root.update()
            m.update()

    except:
        pass

    m.fq.kill()


if __name__ == "__main__":
    main()

