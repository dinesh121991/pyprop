Pyprop Documentation

  1. Installation
    1. [Dependencies](Dependencies.md)
    1. [Installation on Ubuntu](Installation_Ubuntu.md)
    1. Installation on Cray XT4 (hexagon)
      1. [Installation of compute node libraries and applications](Installation_CrayXT4.md)
      1. [Installation of service node applications](Installation_CrayXT4_Pyste.md)

  1. Representations
    * Main Representations
      * CombinedRepresentation
      * CartesianRepresentation
    * Subrepresentations
      * CartesianRepresentation (equidistant grid)
      * BSplineRepresentation / BSplineGridRepresentation
      * ReducedSphericalHarmonicRepresentation / ThetaRepresentation
      * SphericalHarmonicRepresentation / AngularRepresentation
    * Exotic Subrepresentations
      * TransformedRepresentation
      * OrthoPolRepresentation
      * VectorRepresentation

  * Propagators
    * Main Propagators
      * CombinedPropagator
      * CartesianPropagator
      * BasisPropagator
    * Super-propagators
      * PamPropagator
      * ExpokitPropagator
      * OdePropagator
      * RungeKuttaPropagator

  * Solvers
    * ArpackSolver
    * PiramSolver
