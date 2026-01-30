#pragma once

#include "poly.h"
#include <array>

struct EncodedPoly {
    Poly p;

    bool operator==(const EncodedPoly& other) const {
        return p.coeffs == other.p.coeffs;
    }
};
