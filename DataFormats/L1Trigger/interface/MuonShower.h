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
    MuonShower(unsigned bits = 0);

    ~MuonShower();

    // set values
    inline void setBits(unsigned bits) { bits_ = bits; };

    // methods to retrieve values
    inline unsigned getBits() const { return bits_; };

  private:
    unsigned bits_;
  };

}  // namespace l1t

#endif
