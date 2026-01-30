#pragma once
#include <cstddef>

namespace params {
    constexpr size_t poly_degree = 256;   // or 512
    constexpr int32_t q = 12289;            // example modulus
    constexpr size_t seed_bytes = 32;
    constexpr size_t m = 512;
    constexpr size_t n = 256;
}
