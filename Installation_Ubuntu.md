# Introduction #

Ubuntu is the main development platform for pyprop, so installing it there should be as straight forward as possible. Most of the required dependencies are available from synaptic, but some (pypar, blitz and pyste) requires some patching, and must therefore be compiled separately.

**Update:** Patched versions of pypar, blitz and pyste are now bundled with pyprop, in the extern/ folder.

# Details #

The following libraries and applications should be installed from apt
  * build-essential
  * g++-3.4 (g++-4.1 works on Gutsy, Intrepid, Jaunty and Lucid)
  * gfortran
  * gccxml
  * python-dev
  * python-numpy
  * python-numpy-dev (not for Hardy or Intrepid)
  * python-scipy
  * python-tk
  * python-tables
  * python-matplotlib
  * ipython
  * fftw3-dev
  * libgsl0-dev
  * libhdf5-serial-dev
  * libboost-dev
  * libboost-python-dev
  * refblas3-dev (libblas-dev on Intrepid)
  * lapack3-dev (liblapack-dev on Intrepid)
  * python-celementtree (not required on Lucid)
  * python-elementtree (not required on Lucid)
  * openmpi-bin
  * openmpi-dev
  * subversion
  * git-core
  * trilinos (10.x)

## Lucid Lynx ##

To install the required packages listed above (on Lucid Lynx), execute the following
```
sudo apt-get install build-essential gfortran gccxml python-numpy python-numpy-dev python-scipy python-tables python-matplotlib ipython fftw3-dev libgsl0-dev libhdf5-serial-dev libboost-dev libboost-python-dev libblas-dev liblapack-dev liblapack-pic openmpi-bin openmpi-dev subversion python-dev g++-4.1 python-tk git-core
```

## Intrepid Ibex ##

To install the required packages listed above (on Intrepid), execute the following
```
sudo apt-get install build-essential gfortran gccxml python-numpy python-numpy-dev python-scipy python-tables python-matplotlib ipython fftw3-dev libgsl0-dev libhdf5-serial-dev libboost-dev libboost-python-dev libblas-dev liblapack-dev liblapack-pic python-celementtree python-elementtree openmpi-bin openmpi-dev subversion python-dev g++-4.1 python-tk
```

## Jaunty Jackalope ##

Install the same packages as for intrepid, but unfortunately the default pytables version does not work with python2.6, but jhn has a PPA containing pytables 2.1.1:

Add the following lines to /etc/apt/sources.list
```
deb http://ppa.launchpad.net/jenshnielsen/ppa/ubuntu jaunty main
deb-src http://ppa.launchpad.net/jenshnielsen/ppa/ubuntu jaunty main
```
And add the the PGP key to your trusted keys
```
sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 0xac02fab0f4a2300bc78785d009bb1477ccab3297
```
With the new sources in place, run the following to upgrade pytables
```
sudo apt-get update
sudo apt-get upgrade python-tables
```

## Get pyprop ##
In order to get the patches, you must get the pyprop sources from github
```
git clone git://github.com/kvantetore/PyProp.git pyprop
```
Pyprop will then be downloaded to the folder pyprop in the current directory. This folder will be referred to as the pyprop folder

Alternatively, you may fetch the old (unmaintained) version of pyprop from subversion,
```
svn co http://pyprop.googlecode.com/svn/trunk pyprop
```

## Compiling blitz and pypar ##
Recent versions of pyprop have blitz and pypar (and pyste) bundled. To compile these, enter the pyprop/extern folder, and then
```
cd blitz
./configure-blitz
```

to compile blitz. Similarly, for pypar
```
cd pypar
python setup.py build
sudo python setup.py install
```

## Building Trilinos ##
Recently some functionality from Trilinos was incorporated in Pyprop. Therefore, to use certain operations in Pyprop, Trilinos must be available. The main packages we need are Epetra, TPetra, Ifpack and Anasazi.
  * Get [Trilinos](http://trilinos.sandia.gov)
  * The ccmake graphical configuration utility is handy for building trilinos
    1. Untar trilinos to some folder (trilinos)
    1. Make a new folder, trilinos-build
    1. cd trilinos-build
    1. ccmake ../trilinos
    1. Enable required packages, run configure (hit 'c')
    1. When all dependencies have been resolved, hit 'g' to generate makefiles
    1. make
  * Enable Trilinos in Pyprop Makefile.platform: PYPROP\_USE\_TRILINOS := 1, and update CPPFLAGS and LDFLAGS to reflect your local Trilinos installation.

## Installing blitz (deprecated) ##
  1. Get the blitz 0.9 [sources](http://sourceforge.net/project/showfiles.php?group_id=63961)
  1. untar and cd to the blitz source folder
  1. patch blitz with the patch from the pyprop patch folder.
```
patch -p0 < ../pyprop/patch/blitz-propagator.patch
```
  1. configure and make
```
./configure
make lib
sudo make install
```
> > make install will probably fail horribly trying to make the documentation. That can be safely ignored

## Installing pypar (deprecated) ##
  1. Get pypar the pypar [sources](http://datamining.anu.edu.au/~ole/pypar/pypar_1.9.3.tgz)
  1. Untar and cd into the pypar folder
  1. Patch pypar with the patch from the pyprop patch folder
```
patch -p0 < ../pyprop/patch/pypar_1.9.3.patch
```
  1. Compile and install
```
python setup.py build
sudo python setup.py install
```

## Patch pyste (deprecated) ##
  1. Copy the patched pyste files from the pyprop patch folder to the system pyste folder
```
sudo cp patch/pyste/* /var/lib/python-support/python2.5/Pyste/
```
> > If pyste is ever updated through apt, you will need to redo this step