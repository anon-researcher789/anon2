#pragma once
#include "r1cs.h"
#include "bit_gadgets.h"

inline R1CS make_micro_512() {
    R1CS cs;
    cs.num_vars = 512;

    for (size_t i = 0; i < 512; i++) {
        enforce_boolean(cs, i);
    }

    return cs;
}
