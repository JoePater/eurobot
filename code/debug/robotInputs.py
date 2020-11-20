from tkinter import *
from debuggingGUI import RobotButton, RobotSlider

class RobotInputs:
    '''
    fq: a function queue to use for all buttons and sliders

    button_list: a list of lists, each of which will contain
    the text and function for a button

    slider_list: a list of lists, each of which will contain
    the text, function, lower bound and upper bound for a slider
    '''
    def __init__(self, parent, fq, button_list, slider_list):
        self.frame = Frame(parent)
        self.button_list = []
        self.slider_list = []
        for i in button_list:
            r = (RobotButton(self.frame, i[0], i[1], fq))
            r.pack()
            self.button_list.append(r)
        for i in slider_list:
            r = (RobotSlider(self.frame, i[0], i[1], i[2], i[3], fq))
            r.pack()
            self.slider_list.append(r)
            
    def pack(self):
        self.frame.pack()
        
    def grid(self,row=0, column=0, rowspan=1):
        self.frame.grid(column=column, row=row,rowspan=rowspan)
