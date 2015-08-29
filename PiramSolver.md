# Introduction #

The Parallel Implicitly Restarted Arnoldi Method (PIRAM) finds a selection of the eigenpairs of the Hamiltonian by
  1. projecting the Hamiltonian into a Krylov subspace,
  1. calculating the eigenvectors of the projected Hamiltonian,
  1. applying a polynomial filter to the arnoldi vectors (effectively restarting the Arnoldi method with a more suitable start vector)
  1. rebuilding the Krylov subspace.
(2), (3) and (4) are repeated until a desired number of eigenpairs are converged. The method was developed by [Sorensen](http://www.caam.rice.edu/~sorensen/) et al. for the [Arpack](http://www.caam.rice.edu/software/ARPACK/) software package.


# Config Syntax #

The `PiramSolver` shares configuration syntax with the ArpackSolver, and requires an `[Arpack]` section in the configuration file.
```
[Arpack]
krylov_basis_size = 30
krylov_tolerance = 1e-10
krylov_eigenvalue_count = 10
krylov_max_iteration_count = 300
krylov_use_random_start = True
inverse_iterations = True               # Optional
matrix_vector_func = inverseMatVecFunc  # Optional
krylov_eigenvalue_shift = -0.5          # Optional
krylov_debug = False                    # Optional
counter_on = True                       # Optional
```

Required Parameters:
  * `krylov_basis_size` - Size of the Krylov subspace to form.
  * `krylov_tolerance` - Eigenvalue tolerance criterion
  * `krylov_eigenvalue_count` - Number of eigenvalues sought. Must be lower than `krylov_basis_size / 2`
  * `krylov_max_iteration_count` - Maximum number of restart operations before giving up. The total maximum number of matrix-vector multiplications will be `(krylov_basis_size - kyrlov_eigenvalue_count) * krylov_max_iteration_count`
  * `krylov_use_random_start` - Whether to start with a random initial vector for the Krylov subspace, or to use the current wavefuntion as initial vector.

Optional Parameters:
  * `inverse_iterations` - Whether the Krylov subspace of `(H - I sigma)^-1` (shifted inverse) or `H` is calculated. Inverse iterations can be used to accelerate the convergence of eigenvectors near `sigma`, at the cost of having to solve the linear system  `(H - I sigma)` at each iteration in the Arnoldi process instead of multiplying `H` to a wavefunction. If this parameter is `True`, `matrix_vector_func` must also be specified.
  * `matrix_vector_func` - Use an alternative matrix-vector routine instead of the one provided by the Propagator. This is usually used to specify a solver routine such as provided by GMRESShiftInvertSolver when performing inverse iterations.
  * krylov\_eigenvalue\_shift - Shift the eigenvalue spectrum by this amount. **Only specify this when using inverse iterations**.

Debug Parameters:
  * `krylov_debug` - Whether to print out debug info every matrix vector callback.
  * `counter_on` - Whether to print out counter information about number of converged eigenvalues and progress toward maximum number of iterations.