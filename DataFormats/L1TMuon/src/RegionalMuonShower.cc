#include "DataFormats/L1TMuon/interface/RegionalMuonShower.h"

l1t::RegionalMuonShower::RegionalMuonShower() : bits_(0) {}

l1t::RegionalMuonShower::~RegionalMuonShower() {}

bool l1t::RegionalMuonShower::operator==(const l1t::RegionalMuonShower& rhs) const {
  return bits_ == rhs.bits();
  ;
}

uint16_t l1t::RegionalMuonShower::bitsInTime() const { return getDataWord(bits_, kInTimeShift, kInTimeMask); }

uint16_t l1t::RegionalMuonShower::bitsOutTime() const { return getDataWord(bits_, kOutTimeShift, kOutTimeMask); }

bool l1t::RegionalMuonShower::isTwoLooseInTime() const { return bitsInTime() == kTwoLoose; }

bool l1t::RegionalMuonShower::isOneNominalInTime() const { return bitsInTime() == kOneNominal; }

bool l1t::RegionalMuonShower::isTwoLooseOutTime() const { return bitsOutTime() == kTwoLoose; }

bool l1t::RegionalMuonShower::isOneNominalOutTime() const { return bitsOutTime() == kOneNominal; }

void l1t::RegionalMuonShower::setBitsInTime(const uint16_t bits)
{
  setDataWord(bits, bits_, kInTimeShift, kInTimeMask);

}

void l1t::RegionalMuonShower::setBitsOutTime(const uint16_t bits)
{
  setDataWord(bits, bits_, kOutTimeShift, kOutTimeMask);
}

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
