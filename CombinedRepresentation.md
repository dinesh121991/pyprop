# Introduction #

The CombinedRepresentation creates a tensor product of a list of 1D representations. This is the most common representation in PyProp and is suitable for most purposes.


# Config Syntax #

In the `Representation` section, `type` should be set to `"core.CombinedRepresentation"` with `rank` to the number of ranks that should be set up. For each rank, there should be a `representation<rank>` variable pointing to another section, describing the sub-representation for that rank. If several ranks point to the same sub-representation, the sub-representations will be duplicated.

The following example have a two dimensional combined representation.

```
[Representation]
type = "core.CombinedRepresentation"
rank = 2
representation0 = "RepresentationX"
representation1 = "RepresentationY"
```

Each section describing a sub-representation is similar to the `Representation` section, with the limitation of `rank` always being 1. The following example has an equidistant grid (using CartesianRepresentation) from -400 to 400 with 4096 grid points.

```
[RepresentationX]
type = "fourier.CartesianRepresentation"
rank0 = [-400, 400, int(800/0.1953)]

[RepresentationY]
type = "fourier.CartesianRepresentation"
rank0 = [-400, 400, int(800/0.1953)]
```