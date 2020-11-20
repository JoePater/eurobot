import funcQueue
from tkinter import *
import time

def a():
    print("Hello")

class SingleCodeRunner:
    def __init__(self,parent,fq,globs):
        self.fq = fq
        self.globs = globs
        self.frame = Frame(parent,bd=5)
        self.entry = Entry(self.frame,width=30)
        self.button = Button(self.frame,text="Run",command=self.run)

        self.entry.grid(row=0,column=0)
        self.button.grid(row=0,column=1)
        
    def run(self):
        import testMain
        t = self.entry.get()
        f = lambda: eval(t,self.globs)
        self.fq.add(f)

    def pack(self):
        self.frame.pack()

    def grid(self,row=0,column=0):
        self.frame.grid(row=row,column=column)
        
class CodeRunner:
    def __init__(self,parent,fq,n,globs):
        self.fq = fq
        self.frame = Frame(parent)
        
        self.runners = []
        for i in range(n):
            scr = SingleCodeRunner(self.frame,self.fq,globs)
            scr.pack()
            self.runners.append(scr)

    def pack(self):
        self.frame.pack()

    def grid(self,row=0,column=0,rowspan=1):
        self.frame.grid(row=row,column=column,rowspan=rowspan)

def testCodeRunner():
    root = Tk()
    
    fq = funcQueue.FunctionQueue([])
    fq.start()
    
    cr = CodeRunner(root,fq,10,globals())
    
    root.mainloop()
    
    fq.kill()


