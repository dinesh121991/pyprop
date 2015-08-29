#labels Featured
Development TODO list

  * Combine the two blitzblas.h (core/utility/blitzblas.h and core/krylov/piram/piram/blitzblas.h)
    * The blitzblas in piram is much more complete, is better structured, and supports more options. It does, however, only support CBLAS, and the one in utility does some tricks to deal with row-major (C-style) storage.
    * The probable solution is replace utility/blitzblas.h with the one from piram, and modify all places it is used to send in the proper transpose parameters.

  * Parallelize SolveForOverlapMatrix, and InnerProduct (for non-orthogonal basises)
    * Must parallelize the backward substitution of the Cholesky factorization