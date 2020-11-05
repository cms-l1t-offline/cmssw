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

  typedef ObjectRefBxCollection<MuonShower> MuonShowerRefBxCollection;
  typedef ObjectRefPair<MuonShower> MuonShowerRefPair;
  typedef ObjectRefPairBxCollection<MuonShower> MuonShowerRefPairBxCollection;

  class MuonShower : public L1Candidate {
  public:
    MuonShower() {}

    ~MuonShower() {}

    // set values
    inline void setDebug(bool debug) { debug_ = debug; }

    // methods to retrieve values
    inline bool debug() const { return debug_; }

    virtual bool operator==(const l1t::MuonShower& rhs) const { return bits_ == rhs.bits(); }
    virtual inline bool operator!=(const l1t::MuonShower& rhs) const { return !(operator==(rhs)); }

    unsigned int bits() const { return bits_; }

  private:
    // additional hardware quantities only available if debug flag is set
    bool debug_;
    unsigned int bits_;
  };

}  // namespace l1t

#endif
