#pragma once
#include "r1cs.h"
#include "bit_gadgets.h"

struct SHA256Circuit {
    R1CS cs;
    size_t input_offset;
    size_t output_offset;
};

inline SHA256Circuit make_sha256() {
    SHA256Circuit out;
    auto& cs = out.cs;

    // Input: 512 bits
    out.input_offset = 0;
    cs.num_vars = 512;

    for (size_t i = 0; i < 512; i++)
        enforce_boolean(cs, i);

    // Internal state (8 × 32 bits)
    size_t state_base = cs.num_vars;
    cs.num_vars += 256;

    // Output: 256 bits
    out.output_offset = cs.num_vars;
    cs.num_vars += 256;

    // Booleanity for state + output
    for (size_t i = state_base; i < cs.num_vars; i++)
        enforce_boolean(cs, i);


    return out;
}
