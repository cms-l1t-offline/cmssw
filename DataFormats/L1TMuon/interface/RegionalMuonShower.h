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
    enum TypeMasks { kTwoLooseMask = 0x1, kOneNominalMask = 0x1 };
    enum TypeShifts { kTwoLooseShift = 0, kOneNominalShift = 1 };

    RegionalMuonShower();

    ~RegionalMuonShower();

    inline void setDebug(bool debug) { debug_ = debug; };
    inline bool debug() const { return debug_; };
    /// Get link on which the MicroGMT receives the candidate
    const int link() const { return m_link; };

    bool operator==(const l1t::RegionalMuonShower& rhs) const;
    inline bool operator!=(const l1t::RegionalMuonShower& rhs) const { return !(operator==(rhs)); };

    void setDataWord(const uint16_t newWord, uint16_t& word, const unsigned shift, const unsigned mask);
    uint16_t getDataWord(const uint16_t word, const unsigned shift, const unsigned mask) const;

    unsigned int bits() const { return bits_; }
    bool isValid() const { return bits_ > 0; }
    bool isTwoLoose() const;
    bool isOneNominal() const;
    int endcap() const { return endcap_; }
    int sector() const { return sector_; }

    void set_bits(const unsigned bits) { bits_ = bits; }
    void set_endcap(const int endcap) { endcap_ = endcap; }
    void set_sector(const unsigned sector) { sector_ = sector; }
    void set_oneNominal(const bool bit);
    void set_twoLoose(const bool bit);
    void setLink(int link) { m_link = link; };

  private:
    bool debug_;
    uint16_t bits_;
    int endcap_;       //    +/-1.  For ME+ and ME-.
    unsigned sector_;  //  1 -  6.
    int m_link;
  };

}  // namespace l1t

#endif
