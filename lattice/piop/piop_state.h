#include "piop.h"
#include <vector>
struct PIOPProverState {
    Randomness fs_rho;

    EncodedPoly witness;

    std::vector<PIOPCommitment> round1_commitments;
    std::vector<PIOPCommitment> round2_commitments;

    // challenges derived via FS
    int32_t beta;
    int32_t gamma;
};
