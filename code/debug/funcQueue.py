import threading
import time
import variableTracker

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
        
        self.queue_cond.release() #release lock during call
        print("Function returned: ",f())
        self.queue_cond.acquire()

        for v in self.variables:
            v.update()
        
        return True #success

    def __consumer(self):
        while True:            
            self.queue_cond.acquire()

            if len(self.queue): #only one at a time to avoid kill being starved
                self.pop_func()
            else:
                self.queue_cond.wait() #wait for new item
            
            self.queue_cond.release()
                
            self.killed_lock.acquire()
            dead = self.killed
            self.killed_lock.release()
            if dead:
                return

    def start(self):
        self.killed_lock.acquire()

        if not self.killed:
            self.killed_lock.release()
            return
        self.killed = False
            
        self.killed_lock.release()
    
        thr = threading.Thread(target=self.consumer)
        thr.start()

    def kill(self):
        self.killed_lock.acquire()
        self.killed = True
        self.killed_lock.release()

        #notify
        self.queue_cond.acquire()
        self.queue_cond.notify()
        self.queue_cond.release()
        
    def add(self,func):
        self.queue_cond.acquire()
        self.queue.append(func)
        self.queue_cond.notify()
        self.queue_cond.release()
        
def f1():
    print("f1 starting")
    time.sleep(1)
    print("f1 done")

def f2():
    print("f2 starting")
    time.sleep(2)
    print("f2 done")

def f3():
    print("f3 starting")
    time.sleep(3)
    print("f3 done")
    
def test_func_queue():
    fq = FunctionQueue()
    fq.start()

    fq.add(f1)
    fq.add(f2)
    fq.add(f3)

    time.sleep(2)

    fq.add(f1)

    time.sleep(10)

    fq.kill()


    
