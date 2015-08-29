# Introduction #

The paralell arnoldi method propagator (pamp) propagates the wavefunction by projecting the Hamiltonian into a Kyrlov subspace of a given size. A direct calculation of the matrix exponential is performed in the Krylov subspace by either a Padè apprximation or by calculating the eigenpairs of the projected Hamiltonian.

# Config Syntax #

In the project, import the pamp module:
```
import pyprop.modules.solvers.krylov.pamp as pamp
```

The `PamPropagator` can then be used as the `propagator` in the config file:
```
[Propagation]
propagator = pamp.PamPropagator
base_propagator = ...
krylov_basis_size = 10
krylov_exponentiation_method = pamp.ExponentiationMethod.Pade # Optional
krylov_double_orthogonalization = False                       # Optional
```

  * `base_propagator` - A propagator providing `MultiplyHamiltonian`. Usually CombinedPropagator or BasisPropagator.
  * `krylov_basis_size` - the size of the Krylov subspace to form
  * `krylov_exponentiation_method` specifies which exponentiation method to use for the projected Hamiltonian
    * `ExponentiationMethod.Pade` - use a Padè approximation of order 6 (default).
    * `ExponentiationMethod.Eigenvector` - diagonalize the projected Hamiltonian and calculate the exponentiation directly.
  * `krylov_double_orthogonalization` specifies whether a double orthogonalization step should be performed at each step in the Arnoldi method. Defaults to false.