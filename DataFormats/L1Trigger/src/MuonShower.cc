#include "DataFormats/L1Trigger/interface/MuonShower.h"

l1t::MuonShower::MuonShower(unsigned bits)
    : L1Candidate(math::PtEtaPhiMLorentzVector{0., 0., 0., 0.}, 0., 0., 0., 0, 0), bits_(bits) {}

l1t::MuonShower::~MuonShower() {}
