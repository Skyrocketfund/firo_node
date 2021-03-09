#include "schnorr_prover.h"

namespace lelantus {

SchnorrProver::SchnorrProver(const GroupElement& g, const GroupElement& h, bool withFixes_):
    g_(g), h_(h), withFixes(withFixes_) {
}

void SchnorrProver::proof(
        const Scalar& P,
        const Scalar& T,
        const GroupElement& y,
        const GroupElement& a,
        const GroupElement& b,
        unique_ptr<ChallengeGenerator>& challengeGenerator,
        SchnorrProof& proof_out){
    Scalar P0;
    Scalar T0;
    P0.randomize();
    T0.randomize();
    GroupElement u = LelantusPrimitives::commit(g_,P0, h_, T0);
    proof_out.u = u;
    Scalar c;
    std::vector<GroupElement> group_elements = {u};

    std::string shts = "";
    if (withFixes) {
        shts = "SCHNORR_PROOF";
        std::vector<unsigned char> pre(shts.begin(), shts.end());
        group_elements = {u, y, a, b};
        challengeGenerator->add(pre);
    } else {
        challengeGenerator.reset();
    }
    challengeGenerator->add(group_elements);
    challengeGenerator->get_challenge(c);
    proof_out.P1 = P0 - c * P;
    proof_out.T1 = T0 - c * T;
}

}//namespace lelantus