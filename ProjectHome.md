Pyprop is a farily general framework for solving TDSE-like initial value problems, and related problems. The aim is to have a standard framework taking care of parallelization, loading and saving the wavefunction, etc. Discretization schemes and propagation methods are implemented following a standard interface, which enables users to test different methods without requiring detailed knowledge about the implementation.

Propagators for N-dimensional Cartesian coordinates, as well as 3D spherical coordinates are included in the standard package. In addition representations can be combined to create more exotic systems, such as including radial nuclear motion in a diatomic molecule (3D + 1D), combining 3 radial dimensions to simulate a 1D model for a 3-electron atom (1D + 1D + 1D).

Pyprop is written in Python (depending heavily on the numpy module), while all the computationally intensive routines have been developed in C++ (using the blitz++ array library)

The development branch of pyprop has been moved to github http://github.com/kvantetore/PyProp, but the documentation here is still up to date

## Pyprop modules ##
Some extra functionality for specific situations is implemented in plug-in modules for Pyprop. Two are available at the moment, from Github:

  * [EinPartikkel](http://github.com/nepstad/einelektron) - 3D calculations with one electron
  * [Helium](http://github.com/nepstad/pyprop-helium) - 6D calculations with two electrons