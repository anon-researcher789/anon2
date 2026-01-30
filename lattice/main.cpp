#include <iostream>
#include <cassert>
#include <array>
//#include "commitment.cpp"
#include "commitment.h"
#include "encoded_poly.h"
#include "params.h"
#include "piop/piop.cpp"
#include "bit_gadgets.h"
#include "r1cs.h"
#include "sha256.h"
#include "sha512.h"
#include "small_512.h"
// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------
EncodedPoly make_test_poly(int32_t base) {
    EncodedPoly f;
    f.p.coeffs.fill(0);
    f.p.coeffs[0] = base;
    return f;
}
std::array<uint8_t, params::seed_bytes> make_seed(uint8_t v) {
    std::array<uint8_t, params::seed_bytes> seed{};
    seed.fill(v);
    return seed;
}

// ------------------------------------------------------------
// Tests
// ------------------------------------------------------------
void test_keygen_determinism() {
    auto seed = make_seed(42);

    CommitmentKey ck1 = generate_ck(seed);
    CommitmentKey ck2 = generate_ck(seed);

    assert(*ck1.A == *ck2.A);
    assert(*ck1.G == *ck2.G);
    assert(*ck1.T == *ck2.T);

    std::cout << "[OK] Deterministic key generation\n";
}

void test_commit_and_verify() {
    auto seed = make_seed(1);
    CommitmentKey ck = generate_ck(seed);

    EncodedPoly f = make_test_poly(5);
    auto rho = make_seed(9);

    Commitment c = commit(ck, f, rho);

    Opening o{f, rho};

    assert(verify(ck, c, o));

    std::cout << "[OK] Commit / verify\n";
}

void test_commit_fails_on_wrong_opening() {
    auto seed = make_seed(2);
    CommitmentKey ck = generate_ck(seed);

    EncodedPoly f = make_test_poly(3);
    auto rho = make_seed(7);

    Commitment c = commit(ck, f, rho);

    // Tamper with opening
    EncodedPoly bad_f = f;
    bad_f.p.coeffs[0] += 1;

    Opening bad_o{bad_f, rho};

    assert(!verify(ck, c, bad_o));

    std::cout << "[OK] Binding (bad opening rejected)\n";
}
void test_piop_single_witness() {
    std::array<uint8_t, params::seed_bytes> seed{};
    seed[0] = 42;

    CommitmentKey ck = generate_ck(seed);

    EncodedPoly w = make_test_poly(3);
    Randomness rho{};
    rho.seed[0] = 9;

    PIOPProverState pst{ w, rho };
    auto com = piop_prover_round1(ck, pst);

    auto chal = piop_verifier_round2();
    auto op = piop_prover_round3(pst, chal);

    assert(piop_verify(ck, com, op));
    std::cout << "[OK] PIOP single witness\n";
}
void test_fiat_shamir_determinism() {
    Transcript t1("test");
    Transcript t2("test");

    uint64_t x = 42;
    t1.absorb_u64(x);
    t2.absorb_u64(x);

    auto r1 = t1.challenge_randomness("rho");
    auto r2 = t2.challenge_randomness("rho");

    assert(r1.bytes() == r2.bytes());
    std::cout << "[OK] Fiat-Shamir determinism\n";
}
bool eval_lc(
    const LinearCombination& lc,
    const std::vector<int64_t>& w
) {
    int64_t acc = lc.constant;
    for (const auto& t : lc.terms) {
        acc += t.coeff * w[t.var];
    }
    return acc;
}

bool check_r1cs(
    const R1CS& cs,
    const std::vector<int64_t>& witness
) {
    for (const auto& c : cs.constraints) {
        int64_t A = eval_lc(c.A, witness);
        int64_t B = eval_lc(c.B, witness);
        int64_t C = eval_lc(c.C, witness);

        if (A * B != C) {
            return false;
        }
    }
    return true;
}
std::vector<int64_t> make_micro_witness() {
    std::vector<int64_t> w(512);
    for (size_t i = 0; i < 512; i++) {
        w[i] = (i % 2); // alternating bits
    }
    return w;
}
std::vector<int64_t> make_sha256_witness(const SHA256Circuit& c) {
    std::vector<int64_t> w(c.cs.num_vars);

    for (size_t i = 0; i < w.size(); i++) {
        w[i] = (i % 2);
    }
    return w;
}
std::vector<int64_t> make_sha512_witness(const SHA512Circuit& c) {
    std::vector<int64_t> w(c.cs.num_vars);

    for (size_t i = 0; i < w.size(); i++) {
        w[i] = (i & 1);
    }
    return w;
}


// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main() {
    std::cout << "Running lattice commitment tests...\n";

    test_keygen_determinism();
    test_commit_and_verify();
    test_commit_fails_on_wrong_opening();
    test_piop_single_witness();
    test_fiat_shamir_determinism();
    std::cout << "All tests passed ✔\n";
    return 0;
}
// uncomment this for circuit tests
/*
int main() {
    std::cout << "Running circuit sanity checks...\n";

    // ----------------------------
    // 512-bit micro circuit
    // ----------------------------
    {
        auto cs = make_micro_512();
        auto w = make_micro_witness();

        assert(check_r1cs(cs, w));
        std::cout << "[OK] 512-bit micro circuit\n";
        std::cout << "  vars: " << cs.num_vars
                  << " constraints: " << cs.constraints.size() << "\n";
    }

    // ----------------------------
    // SHA-256
    // ----------------------------
    {
        auto sha = make_sha256();
        auto w = make_sha256_witness(sha);

        assert(check_r1cs(sha.cs, w));
        std::cout << "[OK] SHA-256 circuit (structure only)\n";
        std::cout << "  vars: " << sha.cs.num_vars
                  << " constraints: " << sha.cs.constraints.size() << "\n";
    }

    // ----------------------------
    // SHA-512
    // ----------------------------
    {
        auto sha = make_sha512();
        auto w = make_sha512_witness(sha);

        assert(check_r1cs(sha.cs, w));
        std::cout << "[OK] SHA-512 circuit (structure only)\n";
        std::cout << "  vars: " << sha.cs.num_vars
                  << " constraints: " << sha.cs.constraints.size() << "\n";
    }

    std::cout << "All circuit checks passed.\n";
    return 0;
}
*/

// deprecated tests - ignore
// void test_key_update_and_verification() {
//     CommitmentKey ck = generate_ck(make_seed(11));

//     UpdateCircuit uc;
//     uc.delta = 2;
//     uc.rho_update = make_seed(99);

//     CommitmentKey ck_updated = update_ck(std::move(ck), uc);

//     assert(verify_update(ck, ck_updated, uc));

//     std::cout << "[OK] Public key update verification\n";
// }

// void test_commit_survives_key_update() {
//     CommitmentKey ck = generate_ck(make_seed(21));

//     EncodedPoly f = make_test_poly(7);
//     auto rho = make_seed(33);

//     Commitment c = commit(ck, f, rho);

//     UpdateCircuit uc;
//     uc.delta = 1;
//     uc.rho_update = make_seed(44);

//     CommitmentKey ck_updated = update_ck(std::move(ck), uc);

//     // Old commitment should NOT verify under new key
//     Opening o{f, rho};
//     assert(!verify(ck_updated, c, o));

//     std::cout << "[OK] Commitments bound to key version\n";
// }
