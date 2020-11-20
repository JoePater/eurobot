from tkinter import *
from debuggingGUI import RobotButton,RobotSlider,RobotLabels
from codeRunner import CodeRunner
from funcQueue import FunctionQueue,FunctionQueueWidget
from variableTracker import VariableTracker
from robotInputs import RobotInputs

"""

   +-----------------+--------------------+--------------------+
   |                 |                    |                    |
   | Function queue  |                    |                    |
   |                 |                    |                    |
   +-----------------+                    |    RobotInputs     |
   |                 |                    | (ie buttons +      |
   |                 |   Code runner      |      sliders)      |
   |                 |                    |                    |
   |  Variables      |                    |                    |
   |                 |                    |                    |
   |                 |                    |                    |
   |                 |                    |                    |
   |                 |                    |                    |
   +-----------------+--------------------+--------------------+


"""

total_num_coderunners = 8

class MainWindow:
    def __init__(self,parent,rbuttons,rsliders,varfuncs,globs):
        self.vt = VariableTracker(varfuncs)
        self.fq = FunctionQueue([self.vt])

        self.frame = Frame(parent)

        self.fq_widget = FunctionQueueWidget(self.frame,self.fq)
        self.labels = RobotLabels(self.frame)

        self.coderunner = CodeRunner(self.frame,
                                     self.fq,total_num_coderunners,globs)

        self.inputs = RobotInputs(self.frame,self.fq,rbuttons,rsliders)

        self.fq_widget.grid(row=0,column=0)
        self.labels.grid(row=1,column=0)
        self.coderunner.grid(row=0,column=1,rowspan=2)
        self.inputs.grid(row=0,column=2,rowspan=2)

        self.fq.start()

    def pack(self):
        self.frame.pack()

    def grid(self,row=0,column=0):
        self.frame.grid(row=row,column=column)
        
    def update(self):
        self.fq_widget.update()
        self.labels.update(self.vt.get_variables())

def test():
    root = Tk()
    m = MainWindow(root,[],[],{},globals())
    m.pack()
    
    try:
        while 1:
            root.update()
            m.update()
    except:
        pass
    
    m.fq.kill()

