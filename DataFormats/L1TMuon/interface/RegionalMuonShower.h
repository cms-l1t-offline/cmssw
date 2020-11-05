#ifndef DataFormats_L1Trigger_RegionalMuonShower_h
#define DataFormats_L1Trigger_RegionalMuonShower_h

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1Trigger/interface/L1TObjComparison.h"

namespace l1t {

  class RegionalMuonShower;
  typedef BXVector<RegionalMuonShower> RegionalMuonShowerBxCollection;
  /* typedef edm::Ref<RegionalMuonShowerBxCollection> RegionalMuonShowerRef; */
  /* typedef edm::RefVector<RegionalMuonShowerBxCollection> RegionalMuonShowerRefVector; */
  /* typedef std::vector<RegionalMuonShowerRef> RegionalMuonShowerVectorRef; */

  typedef ObjectRefBxCollection<RegionalMuonShower> RegionalMuonShowerRefBxCollection;
  typedef ObjectRefPair<RegionalMuonShower> RegionalMuonShowerRefPair;
  typedef ObjectRefPairBxCollection<RegionalMuonShower> RegionalMuonShowerRefPairBxCollection;

  class RegionalMuonShower {
  public:
    RegionalMuonShower();

    ~RegionalMuonShower();

    inline void setDebug(bool debug) { debug_ = debug; };
    inline bool debug() const { return debug_; };

    bool operator==(const l1t::RegionalMuonShower& rhs) const;
    inline bool operator!=(const l1t::RegionalMuonShower& rhs) const { return !(operator==(rhs)); };

    unsigned int bits() const { return bits_; }
    int endcap() const { return endcap_; }
    int sector() const { return sector_; }

  private:
    bool debug_;
    unsigned int bits_;
    int endcap_;         //    +/-1.  For ME+ and ME-.
    int sector_;         //  1 -  6.
  };

}  // namespace l1t

#endif
