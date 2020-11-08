from tkinter import *
import time
import funcQueue

root = Tk()

class RobotButton():
    def __init__(self, Rwindow, Rtext, Rfunction, FQ):
        self.Rwindow = Rwindow
        self.Rtext = Rtext
        self.Rfunction = Rfunction
        self.newButton = Button(Rwindow, text = Rtext, command=self.runFunction)
        self.FQ = FQ

    def runFunction(self):
        self.FQ.add(self.Rfunction)


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


class RobotSlider():
    def __init__(self, buttonName, buttonFunction, lowerBound, upperBound, FQ):
        self.buttonName = buttonName
        self.slider = Scale(root, from_ = lowerBound, to = upperBound, orient = HORIZONTAL)
        self.buttonFunction = buttonFunction
        self.slideButton = Button(root, text = buttonName, command = self.runFunction)
        self.FQ = FQ

    def runFunction(self):
        self.FQ.add(lambda: self.buttonFunction(self.slider.get()))


#-------------------------TESTING STUFF-------------------------

FQ = funcQueue.FunctionQueue([])

#slider test
def printNum(num):
    print(num + 10)
sli = RobotSlider("yes", printNum, 0, 100, FQ)
sli.slider.pack()
sli.slideButton.pack()

FQ.start()
root.mainloop()

'''
#button tests
def clawUp():
    print("up")

def getRGB():
    RGB = input("Enter RGB: ")
    return RGB


#labels tests
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
'''

