#include "DataFormats/L1Trigger/interface/MuonShower.h"

l1t::MuonShower::MuonShower(unsigned bits)
    : L1Candidate(math::PtEtaPhiMLorentzVector{0., 0., 0., 0.}, 0., 0., 0., 0, 0), bits_(bits) {}

l1t::MuonShower::~MuonShower() {}

uint16_t l1t::MuonShower::bitsInTime() const { return getDataWord(bits_, kInTimeShift, kInTimeMask); }

uint16_t l1t::MuonShower::bitsOutTime() const { return getDataWord(bits_, kOutTimeShift, kOutTimeMask); }

bool l1t::MuonShower::isTwoLooseInTime() const { return bitsInTime() == kTwoLoose; }

bool l1t::MuonShower::isOneNominalInTime() const { return bitsInTime() == kOneNominal; }

bool l1t::MuonShower::isTwoLooseOutTime() const { return bitsOutTime() == kTwoLoose; }

bool l1t::MuonShower::isOneNominalOutTime() const { return bitsOutTime() == kOneNominal; }

void l1t::MuonShower::setBitsInTime(const uint16_t bits)
{
  setDataWord(bits, bits_, kInTimeShift, kInTimeMask);
}

void l1t::MuonShower::setBitsOutTime(const uint16_t bits)
{
  setDataWord(bits, bits_, kOutTimeShift, kOutTimeMask);
}

void l1t::MuonShower::setDataWord(const uint16_t newWord,
                                  uint16_t& word,
                                  const unsigned shift,
                                  const unsigned mask) {
  // clear the old value
  word &= ~(mask << shift);

  // set the new value
  word |= newWord << shift;
}

uint16_t l1t::MuonShower::getDataWord(const uint16_t word, const unsigned shift, const unsigned mask) const {
  return (word >> shift) & mask;
}
