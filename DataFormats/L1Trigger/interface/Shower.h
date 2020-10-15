#ifndef DataFormats_L1Trigger_Shower_h
#define DataFormats_L1Trigger_Shower_h

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/L1Trigger/interface/L1Candidate.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1Trigger/interface/L1TObjComparison.h"

namespace l1t {

  class Shower;
  typedef BXVector<Shower> ShowerBxCollection;
  typedef edm::Ref<ShowerBxCollection> ShowerRef;
  typedef edm::RefVector<ShowerBxCollection> ShowerRefVector;
  typedef std::vector<ShowerRef> ShowerVectorRef;

  typedef ObjectRefBxCollection<Shower> ShowerRefBxCollection;
  typedef ObjectRefPair<Shower> ShowerRefPair;
  typedef ObjectRefPairBxCollection<Shower> ShowerRefPairBxCollection;

  class Shower : public L1Candidate {
  public:
    Shower();

    ~Shower();

    inline void setDebug(bool debug) { debug_ = debug; };
    inline bool debug() const { return debug_; };

    bool operator==(const l1t::Shower& rhs) const;
    inline bool operator!=(const l1t::Shower& rhs) const { return !(operator==(rhs)); };

  private:
    bool debug_;
  };

}  // namespace l1t

#endif
