import threading
import copy

class VariableTracker:
    """
    funcs: a dictionary containing string:function pairs, where the
function will return the value of the variable corresponding to the
string
    variables: a dictionary containing string:value pairs, where the
value of the variable corresponding to the string is value
    """
    def __init__(self,funcs):
        self.funcs = funcs
        self.variables = {}
        self.lock = threading.Lock()

        self.update()

    def add_variable(self,name,func):
        self.lock.acquire()
        self.funcs[name] = func
        self.lock.release()

        self.update()
        
    """
    Should only be called from thread that owns the variables (ie consumer)
    """
    def update(self):
        self.lock.acquire()

        self.variables = {}
        for v in self.funcs:
            self.variables[v] = self.funcs[v]()

        self.lock.release()

    """
    Can be called from either thread
    """
    def get_variables(self):
        self.lock.acquire()
        vars_copy = copy.deepcopy(self.variables[v])
        self.lock.release()

        return vars_copy

    """
    Can be called from either thread
    """
    def get_variable(self,name):
        self.lock.acquire()
        var = copy.deepcopy(self.variables[name])
        self.lock.release()

        return var
