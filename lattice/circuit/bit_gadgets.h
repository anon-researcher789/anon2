#pragma once
#include "r1cs.h"

// Enforce x ∈ {0,1}
inline void enforce_boolean(R1CS& cs, size_t x) {
    // x * (x - 1) = 0
    R1CSConstraint c;
    c.A.terms.push_back({x, 1});
    c.B.terms.push_back({x, 1});
    c.B.constant = -1;
    c.C.constant = 0;
    cs.constraints.push_back(c);
}

// z = x XOR y
inline size_t xor_bit(R1CS& cs, size_t x, size_t y) {
    size_t z = cs.num_vars++;

    // z = x + y - 2xy
    // rearranged: (x + y - z) * 1 = 2xy
    R1CSConstraint c;
    c.A.terms = {{x,1}, {y,1}, {z,-1}};
    c.B.constant = 1;
    c.C.terms = {{x,2}, {y,2}};
    cs.constraints.push_back(c);

    enforce_boolean(cs, z);
    return z;
}
