# Introduction #

Before installing cmake, gccxml and pyste, install all the CNL-libraries described in [Installation\_CrayXT4](Installation_CrayXT4.md).

Because pyste will only run on the service nodes, we do not have to worry about the constraints on that platform. Specifically, we can use shared libraries (yay!). An implication of this is that we should try to keep things that will run on the servicenodes separate from what will run on the compute nodes. I will therefore install everything servicenode related in `/work/torebi/sys_front`. Furthermore, I have created two shell scripts in my home folder, one for compiling software for the CNL, and one for the compiling for the servicenodes.

`~/build_cnl`:
```
export SYS_PATH=/work/torebi/sys_cnl

#Set the  correct python as default
export PYTHONHOME="$SYS_PATH/python"
export PYTHONPATH="$PYTHONHOME/lib/python"
export PATH="$PYTHONHOME/bin:$PATH"

export HDF_DIR="$SYS_PATH/hdf5"

export CC=cc
export CXX=CC
export FC=ftn
export F77=f77
```

`~/build_front`:
```
export SYS_PATH=/work/torebi/sys_front

export CC=gcc
export CXX=g++
export FC=gfortran
export F7=g77
```

So before compiling for the servicenodes, i would run
```
. ~/build_front
```

# Installing cmake #
  1. Get cmake 2.4.8 from [the sources](http://www.cmake.org/HTML/Download.html)
  1. Configure, compile and install
```
./configure --prefix=/work/sys_front/cmake
make
make install
```
  1. Add the following to `~/build_front` and reload it
```
export PATH="$SYS_PATH/cmake/bin:$PATH"
```

# Installing gccxml #
The latest version of gccxml (0.6) is way old, and should not be used. The cvs-version should be used instead. However, some changes was introduced in November/December 2007 which broke pyste. We should therefore check out a version from September 2007
  1. Get gccxml
```
cvs -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML login
cvs -d :pserver:anoncvs@www.gccxml.org:/cvsroot/GCC_XML co -D 2007-09-01 gccxml
```
  1. Configure gccxml to be build in a separate folder
```
mkdir gccxml-build
cd gccxml-build
cmake ../gccxml -i
```
> Press enter to use all default values except for CMAKE\_INSTALL\_PREFIX, where you should change it to `/work/torebi/sys_front/gccxml`
  1. make and install
```
make
make install
```
  1. Add gccxml to the path in `~/build_front`
```
export PATH="$SYS_PATH/gccxml/bin:$PATH"
```

# Installing pyste #
  1. Make sure boost::python is installed
  1. Goto the boost source directory
```
cd libs/python/pyste/install
python setup.py install --prefix=/work/torebi/sys_cnl/python
```
  1. From the pyprop sources, overwrite some of the pyste files from the pyste patch
```
cp <pyprop-dir>/patch/pyste/* /work/torebi/sys_cnl/python/lib/python2.5/site-packages/Pyste/
```

# Installing elementtree #
  1. Get the [sources](http://effbot.org/media/downloads/elementtree-1.2.6-20050316.tar.gz)
  1. Install into both cnl and front python
```
python setup.py install --home=/work/torebi/sys_cnl/python
python setup.py install --home=/work/torebi/sys_front/python
```
> elementtree does not have c-extensions so it is straight forward to install it even in the static python.