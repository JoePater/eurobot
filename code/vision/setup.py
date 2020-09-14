from distutils.core import setup,Extension

"""
Used for compiling and installing python module.

'python setup.py build' will build the program

'sudo python setup.py install' will install it
"""


m1 = Extension("posmodule",sources=["posmodule.cpp"])
setup(name="posmodule",version='0.0',description="Test",ext_modules=[m1])
