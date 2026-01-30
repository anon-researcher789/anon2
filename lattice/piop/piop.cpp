#include "piop.h"

// ------------------------------------------------------------
// Prover Round 1
// ------------------------------------------------------------
PIOPCommitment piop_prover_round1(
    const CommitmentKey& ck,
    PIOPProverState& st
) {
    st.msg1.witness_commitment =
        commit(ck, st.witness, rho.bytes());

    return st.msg1;
}

// ------------------------------------------------------------
// Verifier Round 2
// ------------------------------------------------------------
PIOPChallenge piop_verifier_round2() {
    PIOPChallenge chal;
    chal.zeta = 0; // placeholder
    return chal;
}

// ------------------------------------------------------------
// Prover Round 3
// ------------------------------------------------------------
PIOPOpening piop_prover_round3(
    PIOPProverState& st,
    const PIOPChallenge&
) {
    PIOPOpening op;
    op.witness_opening = {
        st.witness,
        st.rho
    };
    return op;
}

// ------------------------------------------------------------
// Verification
// ------------------------------------------------------------
bool piop_verify(
    const CommitmentKey& ck,
    const PIOPCommitment& com,
    const PIOPOpening& op
) {
    return verify(
        ck,
        com.witness_commitment,
        op.witness_opening
    );
}
