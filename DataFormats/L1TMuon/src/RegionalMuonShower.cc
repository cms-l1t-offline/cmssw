#include "DataFormats/L1TMuon/interface/RegionalMuonShower.h"

l1t::RegionalMuonShower::RegionalMuonShower() : bits_(0) {}

l1t::RegionalMuonShower::~RegionalMuonShower() {}

bool l1t::RegionalMuonShower::operator==(const l1t::RegionalMuonShower& rhs) const {
  return bits_ == rhs.bits();;
}
