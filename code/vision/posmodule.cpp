#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <string>
//#include <iostream>

#include "posmodule.hpp"


int pos_x=5,pos_y=6;
int opp_x=7,opp_y=8;
float bearing=1.234;

Teams team;

static PyObject* set_team(PyObject* self,PyObject* args);
static PyObject* get_team(PyObject* self,PyObject* args);
static PyObject* get_position(PyObject* self,PyObject* args);
static PyObject* get_opponent(PyObject* self,PyObject* args);
static PyObject* get_bearing(PyObject* self,PyObject* args);


static PyMethodDef method_table[] = {
     {"set_team",&set_team,METH_VARARGS,"Set team during init"},
     {"get_team",&get_team,METH_VARARGS,"Get team"},
     {"get_position",&get_position,METH_VARARGS,"Get position"},
     {"get_opponent",&get_opponent,METH_VARARGS,"Get opponent"},
     {"get_bearing",&get_bearing,METH_VARARGS,"Get bearing"},
     {NULL}
};

static struct PyModuleDef posmodule = {
     PyModuleDef_HEAD_INIT,
     "posmodule",
     NULL,
     -1,
     method_table
};

PyMODINIT_FUNC PyInit_posmodule()
{
     return PyModule_Create(&posmodule);
}


static PyObject* set_team(PyObject* self,PyObject* args)
{
     char* str;

     if(!PyArg_ParseTuple(args,"s",&str)){
	  return Py_BuildValue("s","Failed to parse args");
     }

     std::string s{str};

     if(s == "BLUE"){
	  team = Teams::BLUE;
     }
     else if(s == "YELLOW"){
	  team = Teams::YELLOW;
     }
     else{
	  team = Teams::BLUE;
	  //std::cout<<"No matches\n"<<s<<std::endl;
	  return Py_BuildValue("ss","No matching team for ",str);
     }

     Py_INCREF(Py_None);
     return Py_None;
}

static PyObject* get_team(PyObject* self,PyObject* args)
{
     return Py_BuildValue("i",static_cast<int>(team));
}

static PyObject* get_position(PyObject* self,PyObject* args)
{
     PyObject* p = Py_BuildValue("[i,i]",pos_x,pos_y);

     return p;
}

static PyObject* get_opponent(PyObject* self,PyObject* args)
{
     PyObject* p = Py_BuildValue("[i,i]",opp_x,opp_y);

     return p;
}

static PyObject* get_bearing(PyObject* self,PyObject* args)
{
     PyObject* b = Py_BuildValue("f",bearing);

     return b;
}


