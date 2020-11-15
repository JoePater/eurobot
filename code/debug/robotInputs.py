import tkinter
top = tkinter.Tk()

class RobotInputs:
    '''
        Button list of parameters that will be passed for buttons
        the Parameters are Rwindow, Rtext, RFunction
        Widget list is buttonName, ButtonFunction, lower, upperbound
    '''
    def __init__(self, parent, button_list, slider_list):
        self.frame = Frame(parent)
        self.button_list = []
        self.slider_list = []
        for i in button_list:
            r = (RobotButton(self.frame, i[0], i[1], i[2]))
            r.pack()
            self.button_list.append(r)
        for i in slider_list:
            r = (RobotSlider(self.frame, i[0], i[1], i[2], i[3], i[4]))
            r.pack()
            self.widget_list.append(r)
    def pack(self):
        self.frame.pack()
    def grid(row=0, column):
        self.frame.grid(column=column, row=row)
        
        
        
top.mainloop()
