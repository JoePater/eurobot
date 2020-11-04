import funcQueue
from tkinter import *

class SingleCodeRunner:
    def __init__(self,parent,fq):
        self.fq = fq
        self.frame = Frame(parent,bd=5)
        self.entry = Entry(self.frame,width=30)
        self.button = Button(self.frame,text="Run",command=self.run)

        self.entry.grid(row=0,column=0)
        self.button.grid(row=0,column=1)

        self.frame.pack()
        
    def run(self):
        t = self.entry.get()
        f = lambda: exec(t)
        self.fq.add(f)
        
class CodeRunner:
    def __init__(self,parent,fq,n):
        self.fq = fq
        self.frame = Frame(parent)
        
        self.runners = []
        for i in range(n):
            self.runners.append(SingleCodeRunner(self.frame,self.fq))

        self.frame.pack()

root = Tk()
    
fq = funcQueue.FunctionQueue([])
fq.start()

cr = CodeRunner(root,fq,10)

root.mainloop()

fq.kill()
