from tkinter import *

root = Tk()


fuQ = []

def clawUp():
    print("up")

def getRGB():
    RGB = input("Enter RGB: ")
    return RGB

class RobotButton():
    def __init__(self, Rwindow, Rtext, Rfunction):
        self.Rwindow = Rwindow
        self.Rtext = Rtext
        self.Rfunction = Rfunction
        self.newButton = Button(Rwindow, text = Rtext, command=self.runFunction)

    def runFunction(self):
        self.Rfunction()


class RobotLabel():
    def __init__(self, Rwindow, Rfunction):
        self.Rwindow = Rwindow
        self.getAttribute = Rfunction
        self.newLabel = Text(Rwindow)
        self.update()

    def update(self):
        self.Rtext = self.getAttribute()
        self.newLabel.delete('0.0',END)
        self.newLabel.insert('0.0',self.Rtext)
    

BClawUp = RobotButton(root, "Claw up", clawUp)
BRGB = RobotLabel(root, getRGB)
BClawUp.newButton.pack()
BRGB.newLabel.pack()
while True:
    #root.update()
    BRGB.update()
