#pragma once

#include <array>
#include <cstdint>
#include "params.h"

// ------------------------------------------------------------
// Polynomial in R_q = Z_q[X]/(X^N + 1)
// ------------------------------------------------------------
struct Poly {
    static constexpr size_t N = params::poly_degree;

    std::array<int32_t, N> coeffs{};

    int32_t& operator[](size_t i) {
        return coeffs[i];
    }

    const int32_t& operator[](size_t i) const {
        return coeffs[i];
    }
};

// Vector of polynomials (length m)
using PolyVector = std::array<Poly, params::m>;

// ------------------------------------------------------------
// Operators
// ------------------------------------------------------------
inline Poly operator+(const Poly& a, const Poly& b) {
    Poly r;
    for (size_t i = 0; i < params::n; i++)
        r.coeffs[i] = (a.coeffs[i] + b.coeffs[i]) % params::q;
    return r;
}

inline Poly operator*(const Poly& a, const Poly& b) {
    Poly r;
    for (size_t i = 0; i < params::n; i++)
        for (size_t j = 0; j < params::n; j++)
            r.coeffs[(i + j) % params::n] =
                (r.coeffs[(i + j) % params::n]
                + a.coeffs[i] * b.coeffs[j]) % params::q;
    return r;
}

inline Poly operator*(const Poly& a, int32_t scalar) {
    Poly r;
    for (size_t i = 0; i < params::n; i++) {
        int64_t v = static_cast<int64_t>(a.coeffs[i]) * scalar;
        v %= params::q;
        if (v < 0) v += params::q;
        r.coeffs[i] = static_cast<int32_t>(v);
    }
    return r;
}

inline Poly operator*(int32_t scalar, const Poly& a) {
    return a * scalar;
}

inline bool operator==(const Poly& a, const Poly& b) {
    return a.coeffs == b.coeffs;
}
