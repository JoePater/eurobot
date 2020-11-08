import tkinter
import debuggingGUI,funcQueue,variableTracker,codeRunner

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
    def __init__(self,parent,rbuttons,rsliders,varfuncs):
        self.vt = variableTracker.VariableTracker(varfuncs)
        self.fq = funcQueue.FunctionQueue(self.vt)

        self.frame = Frame(parent)

        self.fq_widget = funcQueue.FunctionQueueWidget(self.frame,self.fq)
        self.labels = debuggingGUI.RobotLabels(self.frame)

        self.coderunner = codeRunner.CodeRunner(self.frame,self.fq,total_num_coderunners)

        self.inputs = debuggingGUI.RobotInputs(self.frame,rbuttons + rsliders)

        self.fq_widget.grid(row=0,column=0)
        self.labels.grid(row=1,column=0)
        self.coderunner.grid(row=0,column=1,rowspan=2)
        self.inputs.grid(row=0,column=2,rowspan=2)

    def pack():
        self.frame.pack()

    def grid(row=0,column=0):
        self.frame.grid(row=row,column=column)
        
    def update():
        self.fq_widget.update()
        self.labels.update(self.vt.get_variables)

