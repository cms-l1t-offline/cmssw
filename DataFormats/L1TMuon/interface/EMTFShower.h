#ifndef DataFormats_L1TMuon_EMTFShower_h
#define DataFormats_L1TMuon_EMTFShower_h

namespace l1t {

  class EMTFShower {
  public:
  EMTFShower() :
    bits_(0) {}

    ~EMTFShower() {}

    void setBits(unsigned bits) { bits_ = bits; }
    void set_endcap(int bits) { endcap_ = bits; }
    void set_sector(int bits) { sector_ = bits; }

    unsigned int bits() const { return bits_; }
    int endcap() const { return endcap_; }
    int sector() const { return sector_; }

  private:
    unsigned int bits_;
    int endcap_;         //    +/-1.  For ME+ and ME-.
    int sector_;         //  1 -  6.
  };  // End of class EMTFShower

  // Define a vector of EMTFShower
  typedef std::vector<EMTFShower> EMTFShowerCollection;

}  // End of namespace l1t

#endif
