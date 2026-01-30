#pragma once

#include "poly.h"
#include <array>

template <size_t R, size_t C>
struct PolyMatrix {
    std::array<std::array<Poly, C>, R> data;

    Poly& operator()(size_t i, size_t j) {
        return data[i][j];
    }

    const Poly& operator()(size_t i, size_t j) const {
        return data[i][j];
    }

    bool operator==(const PolyMatrix& other) const {
        for (size_t i = 0; i < R; ++i) {
            for (size_t j = 0; j < C; ++j) {
                if (!(data[i][j].coeffs == other.data[i][j].coeffs)) {
                    return false;
                }
            }
        }
        return true;
    }
};
