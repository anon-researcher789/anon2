#pragma once

#include "poly.h" 
#include "encoded_poly.h"   
#include "matrix.h"

#include <array>

// ------------------------------------------------------------
// Commitment key
// ------------------------------------------------------------
#include <memory>

struct CommitmentKey {
    // Updatable part
    std::unique_ptr<PolyMatrix<params::m, params::n>> A;

    // Fixed public parameters
    std::shared_ptr<const PolyMatrix<params::m, params::n>> G;
    std::shared_ptr<const PolyMatrix<params::m, params::n>> T;

    CommitmentKey(
        std::unique_ptr<PolyMatrix<params::m, params::n>> A_,
        std::shared_ptr<const PolyMatrix<params::m, params::n>> G_,
        std::shared_ptr<const PolyMatrix<params::m, params::n>> T_
    ) : A(std::move(A_)), G(std::move(G_)), T(std::move(T_)) {}

    // move-only
    CommitmentKey(CommitmentKey&&) = default;
    CommitmentKey& operator=(CommitmentKey&&) = default;

    CommitmentKey(const CommitmentKey&) = delete;
    CommitmentKey& operator=(const CommitmentKey&) = delete;
};

// ------------------------------------------------------------
// Commitment + opening
// ------------------------------------------------------------
struct Commitment {
    std::unique_ptr<PolyMatrix<params::m, params::n>> C;
};

struct Opening {
    EncodedPoly f;
    std::array<uint8_t, params::seed_bytes> rho;
};


// ------------------------------------------------------------
// Key update
// ------------------------------------------------------------
struct UpdateCircuit {
    int32_t delta;
    std::array<uint8_t, params::seed_bytes> rho_update;
};

// ------------------------------------------------------------
// API
// ------------------------------------------------------------
CommitmentKey generate_ck(
    const std::array<uint8_t, params::seed_bytes>& seed
);

Commitment commit(
    const CommitmentKey& ck,   // const reference
    const EncodedPoly& f,
    const std::array<uint8_t, params::seed_bytes>& rho
);

bool verify(
    const CommitmentKey& ck,   // const reference
    const Commitment& c,
    const Opening& o
);

CommitmentKey update_ck(
    CommitmentKey&& ck,
    const UpdateCircuit& uc
);

bool verify_update(
    const CommitmentKey& old_ck,
    const CommitmentKey& new_ck,
    const UpdateCircuit& uc
);

