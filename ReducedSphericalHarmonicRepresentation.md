# Introduction #

ReducedSphericalHarmonicRepresentation is for representing a rank expanded in [Spherical Harmics](http://wikipedia.org/Spherical_harmonics), where the `m` quantum number is conserved. The conservation of `m` allows for considerable simplifications. It is no longer necessary to involve the azimuthal angle, which turns the Spherical Harmonics into scaled Legendre Polynomials.


# Module #

The ReducedSphericalHarmonicRepresentation is implemented in the module `pyprop.modules.discretizations.reducedspherical`. The examples here expects that it is imported in the project as `reducedspherical`
```
import pyprop.modules.discretizations.reducedspherical as reducedspherical
```


# Config Syntax #

Set `type` to `reducedspherical.ReducedSphericalHarmonicRepresentation`.

  * `maxl` specifies the maximum value of the angular momentum quantum number `l`.
  * `m` specifies the magnetic quantum number `m`, which is assumed to be constant for this representation.

```
[RepresentationSpherical]
type = "reducedspherical.ReducedSphericalHarmonicRepresentation"
maxl = 10
m = 0
```

The rank will be organized such that `l`s are sorted in ascending order. If `m == 0`, the first data point in the wavefunction corresponds to `l == 0`. If `m != 0` the first data  point will be `l == |m|`, as `l < m` is not valid for Spherical Harmonics