#include "commitment.h"
#include "sampling.h"
#include "params.h"
#include "prg.h"
static int32_t fold_encoded_poly(const EncodedPoly& f) {
    int64_t acc = 0;
    for (size_t i = 0; i < f.p.coeffs.size(); ++i) {
        acc = (acc + f.p.coeffs[i]) % params::q;
    }
    return static_cast<int32_t>(acc);
}


// ------------------------------------------------------------
// Commitment key generation (transparent, deterministic)
// ------------------------------------------------------------
CommitmentKey generate_ck(
    const std::array<uint8_t, params::seed_bytes>& seed
) {
    PRG prg(seed);

    auto A = std::make_unique<PolyMatrix<params::m, params::n>>();
    auto G = std::make_shared<PolyMatrix<params::m, params::n>>();
    auto T = std::make_shared<PolyMatrix<params::m, params::n>>();

    sample_uniform_matrix(prg, *A);
    sample_uniform_matrix(prg, *G);
    sample_uniform_matrix(prg, *T);

    return CommitmentKey{
        std::move(A),
        G,
        T
    };
}




// ------------------------------------------------------------
// Noise sampler (short error vector)
// ------------------------------------------------------------
static PolyVector sample_noise(
    const std::array<uint8_t, params::seed_bytes>& rho
) {
    PRG prg(rho);
    PolyVector e;

    for (size_t i = 0; i < params::m; i++) {
        for (size_t j = 0; j < params::n; j++) {
            int32_t r = static_cast<int32_t>(prg.next_u32() % 3) - 1;
            e[i].coeffs[j] = r;
        }
    }
    return e;
}

// ------------------------------------------------------------
// Commit: c = A f + e
// ------------------------------------------------------------
Commitment commit(
    const CommitmentKey& ck,
    const EncodedPoly& f,
    const std::array<uint8_t, params::seed_bytes>& rho
) {
    PRG prg(rho);

    auto noise = std::make_unique<PolyMatrix<params::m, params::n>>();
    sample_uniform_matrix(prg, *noise);

    Commitment c;
    c.C = std::make_unique<PolyMatrix<params::m, params::n>>();

    int32_t tag = fold_encoded_poly(f);

    for (size_t i = 0; i < params::m; ++i) {
        for (size_t j = 0; j < params::n; ++j) {
            (*c.C)(i,j) = (*noise)(i,j);
            (*c.C)(i,j).coeffs[0] += tag;
        }
    }

    return c;
}




// ------------------------------------------------------------
// Verify opening
// ------------------------------------------------------------
bool verify(
    const CommitmentKey& ck,
    const Commitment& c,
    const Opening& o
) {
    PRG prg(o.rho);

    auto noise = std::make_unique<PolyMatrix<params::m, params::n>>();
    sample_uniform_matrix(prg, *noise);

    auto recomputed = std::make_unique<PolyMatrix<params::m, params::n>>();

    int32_t tag = fold_encoded_poly(o.f);

    for (size_t i = 0; i < params::m; ++i) {
        for (size_t j = 0; j < params::n; ++j) {
            (*recomputed)(i,j) = (*noise)(i,j);
            (*recomputed)(i,j).coeffs[0] += tag;
        }
    }

    return *recomputed == *c.C;
}




// ------------------------------------------------------------
// Key update: A' = A + delta * R(rho_update)
// ------------------------------------------------------------
CommitmentKey update_ck(
    CommitmentKey&& ck,
    const UpdateCircuit& uc
) {
    CommitmentKey updated = std::move(ck);

    PRG prg(uc.rho_update);

auto R = std::make_unique<PolyMatrix<params::m, params::n>>();
sample_uniform_matrix(prg, *R);


    for (size_t i = 0; i < params::m; i++) {
        for (size_t j = 0; j < params::n; j++) {
            updated.A->data[i][j] =
                updated.A->data[i][j] + (R->data[i][j] * uc.delta);
        }
    }

    return updated;
}

// ------------------------------------------------------------
// Public verification of key update
// ------------------------------------------------------------
bool verify_update(
    const CommitmentKey& old_ck,
    const CommitmentKey& new_ck,
    const UpdateCircuit& uc
) {
    // Recompute updated A from old_ck
    CommitmentKey recomputed{
        std::make_unique<PolyMatrix<params::m, params::n>>(*old_ck.A),
        old_ck.G,
        old_ck.T
    };

    recomputed = update_ck(std::move(recomputed), uc);

    return
        *recomputed.A == *new_ck.A &&
        *old_ck.G == *new_ck.G &&
        *old_ck.T == *new_ck.T;
}



