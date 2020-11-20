from tkinter import *
import time
import funcQueue

button_width = 18
labels_width = 40
labels_height = 20
slider_width = 200

class RobotButton():
    def __init__(self, Rwindow, Rtext, Rfunction, FQ):
        self.frame = Frame(Rwindow)
        self.Rtext = Rtext
        self.Rfunction = Rfunction
        self.newButton = Button(self.frame, text = Rtext,
                                command=self.runFunction,
                                width=button_width)
        self.FQ = FQ

        self.newButton.pack()
        
    def runFunction(self):
        self.FQ.add(self.Rfunction)

    def pack(self):
        self.frame.pack()

    def grid(self,row=0, column=0):
        self.frame.grid(column=column,row=row)


class RobotLabels():
    def __init__(self, parent):
        self.frame = Frame(parent)
        self.label = Text(
            self.frame,width=labels_width,height=labels_height)

        self.label.pack()
        
    def update(self, dictionary):
        string = ""
        for i in dictionary:
            string += i + ": " + str(dictionary[i]) +"\n"
        self.label.delete('0.0',END)
        self.label.insert('0.0',string)

    def pack(self):
        self.frame.pack()

    def grid(self,row=0, column=0):
        self.frame.grid(row=row,column=column)


class RobotSlider():
    def __init__(self, parent, buttonName,
                 buttonFunction, lowerBound, upperBound, FQ):
        self.frame = Frame(parent)
        self.buttonName = buttonName
        self.slider = Scale(self.frame, from_ = lowerBound,
                            to = upperBound, orient = HORIZONTAL,
                            length=slider_width)
        self.buttonFunction = buttonFunction
        self.slideButton = Button(self.frame, text = buttonName,
                                  command = self.runFunction,
                                  width=button_width)
        self.FQ = FQ

        self.slider.pack()
        self.slideButton.pack()

    def runFunction(self):
        x = self.slider.get()
        self.FQ.add(lambda: self.buttonFunction(x))

    def pack(self):
        self.frame.pack()

    def grid(self,column=0,row=0):
        self.frame.grid(column=column,row=row)

#-------------------------TESTING STUFF-------------------------
wheels = [0,0]

def set_left_wheel(x):
    global wheels
    wheels[0] = x
    print(f"Setting left wheel to {x}")

def testGUI():
    root = Tk()
    FQ = funcQueue.FunctionQueue([])
    
    sli = RobotSlider(root,"yes", set_left_wheel, 0, 100, FQ)
    sli.pack()
    
    FQ.start()
    root.mainloop()

#testGUI()

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
