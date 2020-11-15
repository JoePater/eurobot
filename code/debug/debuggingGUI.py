from tkinter import *
import time
import funcQueue

root = Tk()

class RobotButton():
    def __init__(self, Rwindow, Rtext, Rfunction, FQ):
        self.frame = Frame(Rwindow)
        self.Rtext = Rtext
        self.Rfunction = Rfunction
        self.newButton = Button(self.frame, text = Rtext, command=self.runFunction)
        self.FQ = FQ

        self.newButton.pack()
        
    def runFunction(self):
        self.FQ.add(self.Rfunction)

    def pack():
        self.frame.pack()

    def grid(row=0, column=0):
        self.frame.grid(column=column,row=row)


class RobotLabels():
    def __init__(self, parent):
        self.frame = Frame(parent)
        self.label = Text(self.frame)

        self.label.pack()
        
    def update(self, dictionary):
        string = ""
        for i in dictionary:
            string += i + ": " + str(dictionary[i]) +"\n"
        self.label.delete('0.0',END)
        self.label.insert('0.0',string)

    def pack():
        self.frame.pack()

    def grid(row=0, column=0):
        self.frame.grid(column=column,row=row)


class RobotSlider():
    def __init__(self, parent, buttonName, buttonFunction, lowerBound, upperBound, FQ):
        self.frame = Frame(parent)
        self.buttonName = buttonName
        self.slider = Scale(self.frame, from_ = lowerBound, to = upperBound, orient = HORIZONTAL)
        self.buttonFunction = buttonFunction
        self.slideButton = Button(self.frame, text = buttonName, command = self.runFunction)
        self.FQ = FQ

        self.slider.pack()
        self.slideButton.pack()

    def runFunction(self):
        self.FQ.add(lambda: self.buttonFunction(self.slider.get()))

    def pack():
        self.frame.pack()

    def grid(column=0,row=0):
        self.frame.grid(column=column,row=row)

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

