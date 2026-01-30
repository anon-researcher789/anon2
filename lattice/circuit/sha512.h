#pragma once
#include "r1cs.h"
#include "bit_gadgets.h"

struct SHA512Circuit {
    R1CS cs;
    size_t input_offset;
    size_t output_offset;
};

inline SHA512Circuit make_sha512() {
    SHA512Circuit out;
    auto& cs = out.cs;

    // Input: 1024 bits
    out.input_offset = 0;
    cs.num_vars = 1024;

    for (size_t i = 0; i < 1024; i++)
        enforce_boolean(cs, i);

    // Internal state (8 × 64 bits)
    size_t state_base = cs.num_vars;
    cs.num_vars += 512;

    // Output: 512 bits
    out.output_offset = cs.num_vars;
    cs.num_vars += 512;

    for (size_t i = state_base; i < cs.num_vars; i++)
        enforce_boolean(cs, i);

    return out;
}
