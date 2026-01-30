#pragma once
#include <vector>
#include <cstdint>

struct LinearTerm {
    size_t var;
    int64_t coeff;
};

struct LinearCombination {
    std::vector<LinearTerm> terms;
    int64_t constant = 0;
};

struct R1CSConstraint {
    LinearCombination A;
    LinearCombination B;
    LinearCombination C;
};

// A * B = C
struct R1CS {
    size_t num_vars;
    std::vector<R1CSConstraint> constraints;
};
