# Introduction #

CartesianRepresentation is somewhat strangely named, and should possibly be be called `EquidistantGridRepresentation` instead. It represents one or more dimensions of equidistant grids combined to a tensor product. CartesianRepresentation is a special case of CombinedRepresentation, but has a different implementation for efficiency reasons.


# Config Syntax #

In the `Representation` section, `type` should be set to `"fourier.CartesianRepresentation"` with `rank` to the number of ranks that should be set up. For each rank, there should be a `rank<rank>` variable describing the parameters of the discretization as a list.
  1. The first element is the start of the grid (including)
  1. The second element is the end of the grid (excluding)
  1. The third element is the number of gridpoints

The following example have a 2D square box of 4096 points in each direction, ranging from (and including) -400 to (and excluding) 400.

```
[Representation]
type = "fourier.CartesianRepresentation"
rank = 2
rank0 = [-400, 400, int(800/0.1953)]
rank1 = [-400, 400, int(800/0.1953)]
```