import sys

sys.path.append("../hardware")

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
