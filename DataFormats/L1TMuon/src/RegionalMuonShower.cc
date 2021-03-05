#include "DataFormats/L1TMuon/interface/RegionalMuonShower.h"

l1t::RegionalMuonShower::RegionalMuonShower() : bits_(0) {}

l1t::RegionalMuonShower::~RegionalMuonShower() {}

bool l1t::RegionalMuonShower::operator==(const l1t::RegionalMuonShower& rhs) const {
  return bits_ == rhs.bits();
  ;
}

bool l1t::RegionalMuonShower::isTwoLoose() const { return getDataWord(bits_, kTwoLooseShift, kTwoLooseMask); }

bool l1t::RegionalMuonShower::isOneNominal() const { return getDataWord(bits_, kOneNominalShift, kOneNominalMask); }

void l1t::RegionalMuonShower::set_oneNominal(const bool bit) {
  setDataWord(bit, bits_, kOneNominalShift, kOneNominalMask);
}

void l1t::RegionalMuonShower::set_twoLoose(const bool bit) { setDataWord(bit, bits_, kTwoLooseShift, kTwoLooseMask); }

void l1t::RegionalMuonShower::setDataWord(const uint16_t newWord,
                                          uint16_t& word,
                                          const unsigned shift,
                                          const unsigned mask) {
  // clear the old value
  word &= ~(mask << shift);

  // set the new value
  word |= newWord << shift;
}

uint16_t l1t::RegionalMuonShower::getDataWord(const uint16_t word, const unsigned shift, const unsigned mask) const {
  return (word >> shift) & mask;
}
