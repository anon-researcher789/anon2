#pragma once

#include "commitment.h"
#include "randomness.h"
#include "encoded_poly.h"

// ------------------------------------------------------------
// PIOP messages
// ------------------------------------------------------------

struct PIOPCommitment {
    std::shared_ptr<const Commitment> value;
};


struct PIOPChallenge {
    int32_t zeta; 
};

struct PIOPOpening {
    Opening witness_opening;
};

// ------------------------------------------------------------
// Prover / Verifier state
// ------------------------------------------------------------

struct PIOPProverState {
    EncodedPoly witness;
    Randomness rho;
    PIOPCommitment msg1;
};

struct PIOPVerifierState {
    PIOPCommitment msg1;
    PIOPChallenge challenge;
};

// ------------------------------------------------------------
// PIOP API
// ------------------------------------------------------------

// Round 1: Prover commits
PIOPCommitment piop_prover_round1(
    const CommitmentKey& ck,
    PIOPProverState& st
);

// Round 2: Verifier sends challenge
PIOPChallenge piop_verifier_round2();

// Round 3: Prover opens
PIOPOpening piop_prover_round3(
    PIOPProverState& st,
    const PIOPChallenge& chal
);

// Verification
bool piop_verify(
    const CommitmentKey& ck,
    const PIOPCommitment& com,
    const PIOPOpening& op
);
