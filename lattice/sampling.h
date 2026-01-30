#pragma once

#include "prg.h"
#include "poly.h"
#include "params.h"

// -----------------------------------------
// Sample a uniform polynomial mod q
// -----------------------------------------
inline Poly sample_uniform_poly(PRG& prg) {
    Poly p;
    for (size_t i = 0; i < Poly::N; ++i) {
        p[i] = static_cast<int32_t>(prg.next_u32() % params::q);
    }
    return p;
}


// -----------------------------------------
// Sample a uniform polynomial matrix
// -----------------------------------------
template<size_t M, size_t N>
void sample_uniform_matrix(PRG& prg, PolyMatrix<M, N>& out) {
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            out(i, j) = sample_uniform_poly(prg);
        }
    }
}
