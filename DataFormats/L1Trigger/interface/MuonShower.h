#ifndef DataFormats_L1Trigger_MuonShower_h
#define DataFormats_L1Trigger_MuonShower_h

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/L1Trigger/interface/L1Candidate.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1Trigger/interface/L1TObjComparison.h"

namespace l1t {

  class MuonShower;
  typedef BXVector<MuonShower> MuonShowerBxCollection;
  typedef edm::Ref<MuonShowerBxCollection> MuonShowerRef;
  typedef edm::RefVector<MuonShowerBxCollection> MuonShowerRefVector;
  typedef std::vector<MuonShowerRef> MuonShowerVectorRef;

  /* typedef ObjectRefBxCollection<MuonShower> MuonShowerRefBxCollection; */
  /* typedef ObjectRefPair<MuonShower> MuonShowerRefPair; */
  /* typedef ObjectRefPairBxCollection<MuonShower> MuonShowerRefPairBxCollection */;

  class MuonShower : public L1Candidate {
  public:
    // Run-3 definitions as provided in DN-20-033
    // One nominal shower considered higher quality than two loose showers
    enum Run3Shower { kInvalid = 0, kTwoLoose = 1, kOneNominal = 2};
    enum BitMask { kInTimeMask = 0x2, kOutTimeMask = 0x2 };
    enum BitShift { kInTimeShift = 0, kOutTimeShift = 2 };

    MuonShower(unsigned bits = 0);

    ~MuonShower();

    void setDataWord(const uint16_t newWord, uint16_t& word, const unsigned shift, const unsigned mask);
    uint16_t getDataWord(const uint16_t word, const unsigned shift, const unsigned mask) const;

    // set values
    void setBits(const unsigned bits) { bits_ = bits; };
    void setBitsInTime(const uint16_t bits);
    void setBitsOutTime(const uint16_t bits);

    // methods to retrieve values
    uint16_t bits() const { return bits_; }
    uint16_t bitsInTime() const;
    uint16_t bitsOutTime() const;

    bool isTwoLooseInTime() const;
    bool isOneNominalInTime() const;
    bool isTwoLooseOutTime() const;
    bool isOneNominalOutTime() const;

  private:
    uint16_t bits_;
  };

}  // namespace l1t

#endif
