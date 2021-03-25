#ifndef DataFormats_L1Trigger_RegionalMuonShower_h
#define DataFormats_L1Trigger_RegionalMuonShower_h

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1Trigger/interface/L1TObjComparison.h"

namespace l1t {

  class RegionalMuonShower;
  typedef BXVector<RegionalMuonShower> RegionalMuonShowerBxCollection;
  typedef ObjectRefBxCollection<RegionalMuonShower> RegionalMuonShowerRefBxCollection;
  typedef ObjectRefPair<RegionalMuonShower> RegionalMuonShowerRefPair;
  typedef ObjectRefPairBxCollection<RegionalMuonShower> RegionalMuonShowerRefPairBxCollection;

  class RegionalMuonShower {
  public:
    // Run-3 definitions as provided in DN-20-033
    // One nominal shower considered higher quality than two loose showers
    enum Run3Shower { kInvalid = 0, kTwoLoose = 1, kOneNominal = 2};
    enum BitMask { kInTimeMask = 0x2, kOutTimeMask = 0x2 };
    enum BitShift { kInTimeShift = 0, kOutTimeShift = 2 };

    RegionalMuonShower();

    ~RegionalMuonShower();

    inline void setDebug(bool debug) { debug_ = debug; };
    inline bool debug() const { return debug_; };

    bool operator==(const l1t::RegionalMuonShower& rhs) const;
    inline bool operator!=(const l1t::RegionalMuonShower& rhs) const { return !(operator==(rhs)); };

    void setDataWord(const uint16_t newWord, uint16_t& word, const unsigned shift, const unsigned mask);
    uint16_t getDataWord(const uint16_t word, const unsigned shift, const unsigned mask) const;

    uint16_t bits() const { return bits_; }
    uint16_t bitsInTime() const;
    uint16_t bitsOutTime() const;

    bool isValid() const { return bits_ > 0; }
    bool isTwoLooseInTime() const;
    bool isOneNominalInTime() const;
    bool isTwoLooseOutTime() const;
    bool isOneNominalOutTime() const;

    int endcap() const { return endcap_; }
    int sector() const { return sector_; }
    /// Get link on which the MicroGMT receives the candidate
    int link() const { return link_; }

    void setBits(const uint16_t bits) { bits_ = bits; }
    void setBitsInTime(const uint16_t bits);
    void setBitsOutTime(const uint16_t bits);

    void setEndcap(const int endcap) { endcap_ = endcap; }
    void setSector(const unsigned sector) { sector_ = sector; }
    void setLink(const int link) { link_ = link; };

  private:
    bool debug_;
    uint16_t bits_;
    int endcap_;       //    +/-1.  For ME+ and ME-.
    unsigned sector_;  //  1 -  6.
    int link_;
  };

}  // namespace l1t

#endif
