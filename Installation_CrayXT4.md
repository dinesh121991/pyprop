# Introduction #

The Cray XT4 uses a simplified linux kernel known as Compute Node Linux (CNL) on its compute nodes. CNL has the advantage that it is very light weight, and therefore has higher performance compared to a full linux kernel. However, this comes at a sacrifice: CNL does not support dynamically shared libraries. This is a real pain for pyprop, as it is based on making small python extensions loaded on run time.

There are two related problems with the lack of dynamic linking. The first is that the required extension modules numpy, pytables, etc. have c-extensions which are normally compiled as shared objects and loaded at runtime. This can be remedied by compiling the c-extensions as static objects, and link them to the python executable at compile time.

The second problem is that examples in pyprop usually has potentials and other functions implemented as an c-extension module using boost python. It is not pratcical to keep a separate python installation for each pyprop application, so we need an alternative.

The Way(TM) to run pyprop on a statically linked system, is first to make a grand libpython.a containing all the required modules like numpy, pytables, etc. and then link a separate executable using libpython.a, a static libpyprop.a as well as the current pyprop projects own module.

My adventures making all this work is described below. My gratitude and respect to the GPAW team for making a brilliant effort into making a statically linked python work more smoothly.

  * Installs everything to /work/torebi/sys\_cnl/, which might not be appropriate for everyone else.

Before starting, make sure to set gcc as the programming environment on hexagon, I have to swap it with the pgi compiler. Furthermore, i whish to use the gfortran compiler which is only available in gcc-4, i therefore swap gcc/3.3.3 (the default gcc on hexagon) with gcc/4.2.1
```
module swap PrgEnv-pgi PrgEnv-gnu
module swap gcc/3.3.3 gcc/4.2.1
```
This can with success be done in .bashrc or similar.
UPDATE: with the quad core installed on hexagon, gcc/4.2.0 is the default gcc on hexagon, so we'll use this.

Second, because we are compiling applications for CNL, set the cnl compilers to be the default
```
export CC=cc
export CXX=CC
export FC=ftn
export F77=f77
```
This will make many programs use the correct compilers by default. However, when compiling for the front nodes, these environment variables should not be set.

UPDATE (27/05/10): Some of these steps may require a little tweaking and creativity when using newer versions of the various libraries (had some problems with numpy 1.4.1 for instance. EDIT: This turned out be caused by linking to the wrong version of libsci\_quadcore.).


# Installing Python #

  1. Get python [soures](http://www.python.org/download/)
  1. Create your own dynamic loader Python/dynload\_redstorm.c (Get it from the GPAW page)
  1. Replace the unix compiler with one that statically links the libs (Get unixccompiler.py from the GPAW page)
  1. Edit Modules/Setup to include at least all standard modules (like cmath, math, time, etc)
  1. Configure
```
./configure --prefix=/work/torebi/sys_cnl/python SO=.a DYNLOADFILE=dynload_redstorm.o MACHDEP=hexagon --host=x86_64-unknown-linux-gnu --disable-sockets --disable-ssl --enable-static --disable-shared
```
  1. Make
  1. Install
```
make install
make bininstall
make inclinstall
make libainstall
```
> in my case make install fails when byte-compiling some of the modules, therefore the additional make installs are needed.

# Installing CBLAS #

It is easier to get numpy working properly (with a high speed BLAS implementation) if we use CBLAS to expose the fortran BLAS functions to C.
  1. Get the cblas [sources](http://www.netlib.org/blas/blast-forum/cblas.tgz) from netlib
  1. Make Makefile.Linux the default makefile
```
ln -s Makefile.Linux Makefile.in
```
  1. Edit Makefile.in in a suitable editor, and change line 26 to
```
CBDIR = /work/torebi/sys_cnl/CBLAS
```
  1. Also change line 35 to
```
FC = ftn
```
  1. Compile and install to /work/torebi/sys\_cnl/CBLAS
```
make alllib
```
  1. Create symlinks in order to be more helpful to the insane distutils script of numpy
```
cd /work/torebi/sys_cnl/CBLAS/lib
ln -s LINUX/cblas_LINUX.a libcblas.a
ln -s /opt/xt-libsci/default/gnu/lib/libsci_quadcore.a libsci.a
```
  1. The exact path to libsci\_quadcore.a, or libsci.a (depending on your XT-system) could be slightly different. You should now have both libsci.a and libcblas.a in the same directory. This is necessary in order to make the blas/lapack-finder in numpy-distutils work.

# Installing numpy #

Due to the smart hack suggested by the GPAW team, distutils will work more or less as for a dynamically linked python, with the difference being that the extension modules will be compiled into static .a libs. These .a files must then be linked with the python executable to make them available to the python runtime.

  1. Get the numpy [sources](http://numpy.scipy.org/) (currently tested on version 1.2.1)
  1. Create a file called site.cfg
```
[DEFAULT]
search_static_first = true

[blas]
blas_libs = cblas, sci
library_dirs = /work/torebi/sys_cnl/CBLAS/lib

[lapack]
lapack_libs = sci
library_dirs = /work/torebi/sys_cnl/CBLAS/lib

[blas_opt]
blas_libs = cblas, sci
libraries = cblas, sci

[lapack_opt]
libraries = sci

[fftw]
libraries = fftw3
```
  1. Run distutils config to see that it found blas and lapack
```
python setup.py install
```
> The output should say, among other things, `blas_info: FOUND`, and `lapack_info: FOUND`
  1. Run distutils as normal
```
python setup.py build
python setup.py install --prefix=/work/torebi/sys_cnl/python
```
> If the build complains about not being able to find `_MAIN`, rerun the build process a couple of times. There is a bug in numpy-distutils which causes it to try to compile the .a-files into executables, but it tries only once for each library file. If this does not work, try manually creating the archive files, i.e. 'ar cr libname.a file1.o file2.o ...'.
> This should build all the necessary c-extensions to numpy and install everything into /work/torebi/sys\_cnl/python/lib/python/numpy.
  1. In the python source directory, add the following to the end of Modules/Setup
```
NUMPY=/work/torebi/sys_cnl/python/lib/python2.5/site-packages
multiarray $(NUMPY)/numpy/core/multiarray.a
umath $(NUMPY)/numpy/core/umath.a
_sort $(NUMPY)/numpy/core/_sort.a
scalarmath $(NUMPY)/numpy/core/scalarmath.a
_compiled_base $(NUMPY)/numpy/lib/_compiled_base.a
_capi $(NUMPY)/numpy/numarray/_capi.a
fftpack_lite $(NUMPY)/numpy/fft/fftpack_lite.a
lapack_lite $(NUMPY)/numpy/linalg/lapack_lite.a
mtrand $(NUMPY)/numpy/random/mtrand.a
```
  1. Do a `make; make bininstall` to make and install the new python executable with the numpy c-extension built in.
  1. Test numpy
```
python
>>> import numpy
>>> x = numpy.r_[0:10]
>>> x**2
```
# Installing boost::python #

  1. Get boost [sources](http://www.boost.org/)
  1. Configure with your favourite prefix and which libraries to compile
```
/configure --prefix=/work/torebi/sys_cnl/boost --with-libraries=python
```
  1. Modify user-config.jam, to force jam to use the CC compiler command instead of g++
```
# Boost.Build Configuration
# Automatically generated by Boost configure 

# Compiler configuration
using gcc : : CC ;

# Python configuration
using python : 2.5 : /work/torebi/sys_cnl/python ;
```
  1. run
```
make
make install
```
> or run jam manually
```
./tools/jam/src/bin.linux/bjam  --user-config=user-config.jam --with-python --prefix=/work/torebi/sys_cnl/boost --layout=system
./tools/jam/src/bin.linux/bjam  --user-config=user-config.jam --with-python --prefix=/work/torebi/sys_cnl/boost --layout=system install
```

# Installing HDF and pytables #

Installing HDF5 is relatively straight forward
  1. Get hdf5 [sources](http://hdf.ncsa.uiuc.edu/HDF5/index.html) (tested with 1.8.0)
  1. configure and make with
```
./configure --prefix=/work/torebi/sys_cnl/hdf5 --disable-shared --enable-static --enable-fortran
make
make install
```

Installing pytables is slightly more tricky
  1. Get pytables [sources](http://www.pytables.org/download/stable/) (tested with 2.0.2)
  1. Because pytables does not yet have full support for the new interface in hdf5 1.8, we must force the 1.6 interface as default
```
export CFLAGS="-D H5_USE_16_API"
```
UPDATE: pytables 2.0.3 supports hdf5 1.8 out of the box, so the above is not necessary
  1. Build and install pytables
```
python setup.py build
python setup.py install --prefix=/work/torebi/sys_cnl/python
```
  1. Like for numpy, we must link the c-extensions to python statically. Go to the python source directory and add the following lines to Modules/Setup
```
PYTABLES=/work/torebi/sys_cnl/python/lib/python2.5/site-packages/tables
interpreter $(PYTABLES)/numexpr/interpreter.a
hdf5Extension $(PYTABLES)/hdf5Extension.a
tableExtension $(PYTABLES)/tableExtension.a
utilsExtension $(PYTABLES)/utilsExtension.a
_comp_lzo $(PYTABLES)/_comp_lzo.a
_comp_bzip2 $(PYTABLES)/_comp_bzip2.a /work/torebi/sys_cnl/hdf5/lib/libhdf5.a -lbz2
```
  1. Recompile and install the python executable
```
make
make bininstall
```

# expat #
UPDATE: Expat is included in Python these days, so this should not be necessary(?)

Expat is an xml parser for python. It is required if we want to use pyste using the statically linked python.
  1. Get the [sources](http://sourceforge.net/projects/expat)
  1. Configure without shared libs
```
./configure --prefix=/work/torebi/sys_cnl/expat --enable-static --disable-shared 
```
  1. Make and install
```
make
make install
```
  1. Enable the expat wrapper module in python. Modify `Modules/Setup` in the python source directory, and add the following lines
```
EXPAT_DIR=/work/torebi/sys_cnl/expat
pyexpat pyexpat.c -DHAVE_EXPAT_H -I$(EXPAT_DIR)/include -L$(EXPAT_DIR) $(EXPAT_DIR)/lib/libexpat.a
```

# pypar #
We need something to start MPI\_Init. Certainly, we could do it our `main()`, but it is nice to have mpi-functionality in python as well (otherwise we'll have to send ProcId and ProcCount out to python at some stage. Pyprop uses pypar to start mpi.
  1. Get pypar 1.9. from the [sources](http://datamining.anu.edu.au/~ole/pypar/pypar_1.9.3.tgz)
  1. Patch pypar with the pyprop patch
```
wget http://pyprop.googlecode.com/svn/trunk/patch/pypar_1.9.3.patch
patch -p0 < pypar_1.9.3.patch
```
  1. Edit `lib/pypar.py`, and change line 696 to include "hexagon" in the platform list
```
...
695 
696     if sys.platform in ['linux2', 'sunos5', 'win32', 'darwin', "hexagon"]:
697         #Linux (LAM,MPICH) or Sun (MPICH)
...
```
  1. Using distutils to compile pypar is  going to end in tears. pypar tries to be really smart about finding out which mpi libraries to use and such. As `cc` supplies all the nescessary include paths to the mpi library, we don't need all that, and we'll be better of compiling and installing pypar manally. It is also a good exercies to get to know the compiling process on the Cray better.
```
rm -f mpiext.o mpiext.a
cc -I/work/torebi/sys_cnl/python/include/python2.5  -I/work/torebi/sys_cnl/python/lib/python2.5/site-packages/numpy/core/include lib/pypar/mpiext.c -o mpiext.o -c
ar cr mpiext.a mpiext.o

mkdir /work/torebi/sys_cnl/python/lib/python2.5/site-packages/pypar
cp mpiext.a /work/torebi/sys_cnl/python/lib/python2.5/site-packages/pypar/
cp lib/pypar/pypar.py /work/torebi/sys_cnl/python/lib/python2.5/site-packages
```
  1. Go to the python source directory, and add mpiext.a to Modules/Setup
```
PYPAR=/work/torebi/sys_cnl/python/lib/python2.5/site-packages/pypar
mpiext $(PYPAR)/mpiext.a
```
  1. Recompile python
```
make
make bininstall
```

If you get a the error message
```
Starting mpi[0] assertion: st == sizeof ident at file mptalps.c line 93, pid <...>
```
it means you are trying to start mpi outside the `aprun` environment. To test pypar, create a file `test.py`
```
import pypar
print "Hello World from %i" % pypar.rank()
```
and test it with
```
aprun -n 16 python test.py
```

# matplotlib #
matplotlib is not strictly necessary to run pyprop, and as interactive python on the Cray is not very pleasant, installing matplot is optional. That said, a lot of the pyprop examples does a `import pylab`, and will fail if matplotlib is not available.

  1. Get the matplotlib [sources](http://matplotlib.sourceforge.net/) (Tested with version 0.91.2)
  1. As we specified a custom platform to python (pyprop), we must add an entry to `basedir` in the top of setupext.py
```
    'hexagon' : ["/usr"],
```
  1. Now matplotlib should compile and install
```
python setup.py build
python setup.py install --prefix=/work/torebi/sys_cnl/python
```
  1. Goto the python source directory and add the following to Modules/Setup
```
#matplotlib
MATPLOTLIB=/work/torebi/sys_cnl/python/lib/python2.5/site-packages/matplotlib
LIBPNG=/usr/lib64/libpng.a
LIBFREETYPE=/usr/lib64/libfreetype.a
ft2font $(MATPLOTLIB)/ft2font.a  $(LIBFREETYPE) -lz -lstdc++ -lm
ttconv $(MATPLOTLIB)/ttconv.a 
_cntr $(MATPLOTLIB)/_cntr.a 
nxutils $(MATPLOTLIB)/nxutils.a 
_agg $(MATPLOTLIB)/_agg.a -lstdc++ -lm
_transforms $(MATPLOTLIB)/_transforms.a -lstdc++ -lm
_backend_agg $(MATPLOTLIB)/backends/_backend_agg.a $(LIBPNG) -lz -lm $(LIBFREETYPE) -lz -lstdc++ -lm
_image $(MATPLOTLIB)/_image.a $(LIBPNG) -lz -lm
ctraits $(MATPLOTLIB)/../enthought/traits/ctraits.a

#datetime module is required by matplotlib
datetime datetimemodule.c
```


# Links #
  * [Installing GPAW on Cray XT4](https://wiki.fysik.dtu.dk/gpaw/Platforms_and_Architectures#id18)
  * [Embedding python in other apps](http://docs.python.org/ext/embedding.html)
  * [Embedding using boost::python](http://www.boost.org/libs/python/doc/tutorial/doc/html/python/embedding.html)