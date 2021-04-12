#ifndef FIRMWARE_dataformats_jets_h
#define FIRMWARE_dataformats_jets_h

#include "TkJetsDataTypes.h"
#include "TkJetsBit_Encoding.h"
#include <vector>

namespace l1t { //l1t

  struct TkJetWord { //change
    pt_t hwPt;
    glbeta_t hwEta;
    glbphi_t hwPhi;
    z0_t hwZ0;
    nt_t hwNt; //number of tracks
    nx_t hwXt; //number of tracks with xbit=1

    inline void clear() {
      hwPt = 0;
      hwEta = 0;
      hwPhi = 0;
    }

    int intPt() const { return Scales::intPt(hwPt); }
    int intNTracks() const { return Scales::intNt(hwNt); }
    int intNXTracks() const { return Scales::intXt(hwXt); }
    float floatPt() const { return Scales::floatPt(hwPt); }
    float floatEta() const { return Scales::floatEta(hwEta); }
    float floatPhi() const { return Scales::floatPhi(hwPhi); }

  };
 
  
  typedef std::vector<l1t::TkJetWord> TkJetEmulationCollection;
  
}  // namespace l1ct

#endif
