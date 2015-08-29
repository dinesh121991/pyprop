# Details #

  1. Create the example test.cpp
```
#include <boost/python.hpp>
using namespace boost::python;
//#include <Python.h>

int main(int argc, char** argv)
{
        //Initialize python
        Py_Initialize();

        //Get the main module
        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__");

        //Run a test command
        exec("print 'hello world'", main_namespace, main_namespace);

        try
        {
                //Run a test file
                exec_file("test.py", main_namespace, main_namespace);
        }
        catch (error_already_set const &)
        {
                PyErr_Print();
        }
}
```
  1. Create a example python file test.py
```
#Simple test
print "Hello world"

#Print out all loaded modules
import sys
for k in sys.modules.keys():
        print k

#Print out all files in the current dir
import os
print os.listdir("/")
```
  1. Create a Makefile and customize it to use the correct paths
```
PYTHON_DIR = /work/torebi/sys_cnl/python
PYTHON_LIB = $(PYTHON_DIR)/lib/libpython2.5.a
PYTHON_INC = -I$(PYTHON_DIR)/include/python2.5

BOOST_DIR = /work/torebi/sys_cnl/boost
BOOST_INC = -I$(BOOST_DIR)/include
BOOST_PYTHON_LIB = $(BOOST_DIR)/lib/libboost_python.a

test: test.o
        CC test.o $(PYTHON_LIB) $(BOOST_PYTHON_LIB) -o test -ldl -lutil -lm -static -static-libgcc

test.o: test.cpp
        CC -c test.cpp -o test.o $(PYTHON_INC) $(BOOST_INC) -static -static-libgcc

clean:
        rm -f test
        rm -f test.o
```