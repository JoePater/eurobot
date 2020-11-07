import threading
import time
import variableTracker
from tkinter import *
import copy

"""
Represents a FunctionQueue object as a widget.
Displays the name of the currently running function, the
number of functions in the queue (excluding the current 
function) and the return value from the previous function
to run
"""
class FunctionQueueWidget:
    def __init__(self,parent,fq=None):
        self.frame = Frame(parent)

        self.nameVar = StringVar()
        self.funcsVar = StringVar()
        self.retVar = StringVar()

        self.name = Label(self.frame,textvariable=self.nameVar)
        self.funcs = Label(self.frame,textvariable=self.funcsVar)
        self.ret = Label(self.frame,textvariable=self.retVar)
        
        if fq is None:
            self.fq = FunctionQueue()
            self.fq.start()
        else:
            self.fq = fq

        self.name.grid(row=0,column=0)
        self.funcs.grid(row=1,column=0)
        self.ret.grid(row=2,column=0)

    def pack(self):
        self.frame.pack()

    def grid(self,row=0,column=0):
        self.frame.grid(row=row,column=column)

    def update(self):
        nam = self.fq.get_curr_func()
        if nam is None:
            self.nameVar.set("Currently idle")
        else:
            self.nameVar.set("Current function: {}".format(nam))
            
        self.funcsVar.set("Queue length: {}".format(self.fq.length()))

        prev = self.fq.get_prev_func()
        if prev is None:
            self.retVar.set("No return value available")
        else:
            self.retVar.set("Return value from '{}': {}".format(prev,self.fq.get_return()))


"""
A class for queuing and running functions in a producer/consumer
pattern. Call start() to begin the consumer thread, then call add()
to add a function to the queue. Call kill() to end the consumer
thread.
"""
class FunctionQueue:
    """
    vt: list of VariableTracker objects to update after calling function
    """
    def __init__(self,vt):
        self.queue = []
        self.ret_val = None
        self.curr_func = None
        self.prev_func = None
        self.queue_cond = threading.Condition()

        self.killed = True
        self.killed_lock = threading.Lock()
        
        self.variables = vt

    """
    queue_cond must be held
    """
    def __pop_func(self):
        #cancel if queue is empty
        if len(self.queue) == 0:
            return False
        
        f = self.queue.pop(0) #get item from queue

        self.curr_func = f.__name__
        
        self.queue_cond.release() #release lock during call
        try:
            ret = f()
        except:
            ret = "Function threw exception"
        self.queue_cond.acquire()

        self.prev_func = self.curr_func
        self.curr_func = None
        self.ret_val = ret

        for v in self.variables:
            v.update()
        
        return True #success

    def __consumer(self):
        while True:            
            self.queue_cond.acquire()

            if len(self.queue): #only one at a time to avoid kill being starved
                self.__pop_func()
            else:
                self.queue_cond.wait() #wait for new item
            
            self.queue_cond.release()

            with self.killed_lock:
                if self.killed:
                    return

    def start(self):
        self.killed_lock.acquire()

        if not self.killed:
            self.killed_lock.release()
            return
        self.killed = False
            
        self.killed_lock.release()
    
        thr = threading.Thread(target=self.__consumer)
        thr.start()

    def kill(self):
        with self.killed_lock:
            self.killed = True

        #notify
        with self.queue_cond:
            self.queue_cond.notify()
        
    def add(self,func):
        with self.queue_cond:
            self.queue.append(func)
            self.queue_cond.notify()

    def get_return(self):
        with self.queue_cond:
            return copy.deepcopy(self.ret_val)

    def length(self):
        with self.queue_cond:
            return len(self.queue)

    def get_curr_func(self):
        with self.queue_cond:
            return copy.deepcopy(self.curr_func)

    def get_prev_func(self):
        with self.queue_cond:
            return copy.deepcopy(self.prev_func)
        
def f1():
    print("f1 starting")
    time.sleep(1)
    print("f1 done")
    return 10

def f2():
    print("f2 starting")
    time.sleep(2)
    print("f2 done")
    return 20

def f3():
    print("f3 starting")
    time.sleep(3)
    print("f3 done")
    return 30
    
def test_func_queue():
    root = Tk()
                          
    fq = FunctionQueue([])
    fq.start()
    fqw = FunctionQueueWidget(root,fq)
    fqw.pack()
    
    root.update()
    
    fq.add(f1)
    fq.add(f2)
    fq.add(f3)

    t0 = time.time()
    while 1:
        root.update()
        fqw.update()

    fq.kill()

test_func_queue()
    
