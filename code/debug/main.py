import sys
import mainWindow
from tkinter import *
import time

sys.path.append("../hardware")

debug_mode = "motors"
update_vars = False

if debug_mode == "motors":
    import bus
    import motors
    
    robot_buttons = [
        ["Enable A",lambda: motors.enableMotor("A")],
        ["Enable B",lambda: motors.enableMotor("B")],
        ["Disable A",lambda: motors.disableMotor("A")],
        ["Disable B",lambda: motors.disableMotor("B")],
    ]

    robot_sliders = [
        ["Set A",lambda x: motors.setSpeed("A",x),-100,100],
        ["Set B",lambda x: motors.setSpeed("B",x),-100,100],
    ]
    
    variable_functions = {
        "MB":lambda: vars(motors.boards[0]),
        "R0":lambda: bin(bus.bus.read_byte_data(0x43,0)),
        "R1":lambda: bus.bus.read_byte_data(0x43,1),
        "t":lambda: time.time(),
    }

elif debug_mode == "revolver":
    import bus
    import motors

    robot_buttons = [
        ["Enable rev",lambda: motors.enableMotor("REVOLVER")],
        ["Disable rev",lambda: motors.disableMotor("REVOLVER")],
    ]

    robot_sliders = [
        ["Revolve",motors.turnRevolver,-5,5],
    ]
    
    variable_functions = {
        "Board":lambda: vars(motors.boards[0]),
        "Time":lambda: time.time(),
        "Steps": lambda: bus.bus.read_byte_data(0x43,4),
        "Photointerrupter": lambda: bus.bus.read_byte_data(0x43,5),
        "Speed": lambda: bus.bus.read_byte_data(0x43,3),
    }
    
elif debug_mode == "buttons":
    import buttons
    
    robot_buttons = []
    robot_sliders = []

    update_vars = True

    variable_functions = {
        "Claw open": lambda: buttons.buttonPressed("CLAW_OPEN"),
        "Right way up": lambda: buttons.buttonPressed("RIGHT_WAY_UP"),
        "Upside down": lambda: buttons.buttonPressed("UPSIDE_DOWN"),
        "Lift up": lambda: buttons.buttonPressed("LIFT_UP"),
        "Lift down": lambda: buttons.buttonPressed("LIFT_DOWN"),
    }

def main():
    root = Tk()
    m = mainWindow.MainWindow(root,robot_buttons,robot_sliders,
                              variable_functions,globals())
    m.pack()

    try:
        while 1:
            root.update()
            m.update(update_vars)

    except:
        pass

    m.fq.kill()


if __name__ == "__main__":
    main()
