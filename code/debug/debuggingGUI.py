from tkinter import *
import time

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


class RobotLabels():
    def __init__(self):
        self.label = Text(root)
        self.label.pack()

    def update(self, dictionary):
        string = ""
        for i in dictionary:
            string += i + ": " + str(dictionary[i]) +"\n"
        self.label.delete('0.0',END)
        self.label.insert('0.0',string)


dictionary1 = {"abc":"xyz","123":"789"}
dictionary2 = {"qwe":"bnm","456":"012","joe":"naeem"}

robotStats = RobotLabels()

while True:
    root.update()
    time.sleep(1)
    robotStats.update(dictionary1)
    root.update()
    time.sleep(1)
    robotStats.update(dictionary2)
    
