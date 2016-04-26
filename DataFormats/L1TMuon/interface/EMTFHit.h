// Class for input trigger primitives to EMTF - AWB 04.01.16
// Based on L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitive.h
// In particular, see struct CSCData

#ifndef __l1t_EMTFHit_h__
#define __l1t_EMTFHit_h__

#include <vector>
#include <boost/cstdint.hpp> 
#include <cmath>

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/L1TMuon/interface/EMTF/ME.h"

namespace l1t {
  class EMTFHit {
  public:
    
  EMTFHit() :
    
    // Using -999 instead of -99 b/c this seems most common in the emulator.  Unfortunate. - AWB 17.03.16
    endcap(-999), station(-999), ring(-999), sector(-999), sector_GMT(-999), 
      subsector(-999), chamber(-999), layer(-999), csc_ID(-999), neighbor(-999), 
      mpc_link(-999), wire(-999), strip(-999), zone_hit(-999), track_num(-999), 
      phi_hit(-999), phi_z_val(-999), phi_loc_int(-999), phi_loc_deg(-999), 
      phi_loc_rad(-999), phi_glob_deg(-999), phi_glob_rad(-999), phi_geom_rad(-999),
      theta_int(-999), theta_loc(-999), theta_deg(-999), theta_rad(-999), eta(-999), 
      quality(-999), pattern(-999), bend(-999), valid(-999), sync_err(-999), 
      bx0(-999), bx(-999), is_CSC_hit(false), is_RPC_hit(false)
      {};
    
    virtual ~EMTFHit() {};

    float pi = 3.141592653589793238;

    // Functions defined in src/EMTFHit.cc
    void ImportCSCDetId ( const CSCDetId& _detId);
    void ImportCSCCorrelatedLCTDigi ( const CSCCorrelatedLCTDigi& _digi);
    void ImportME ( const emtf::ME _ME );

    void SetZoneContribution (std::vector<int> vect_ints)  { zone_contribution = vect_ints; }
    void SetCSCDetId         (CSCDetId id)                 { csc_DetId         = id;        }
    
    std::vector<int> Zone_contribution () const { return zone_contribution; }
    CSCDetId CSC_DetId                 () const { return csc_DetId;         }

    int calc_sector_GMT (int _endcap, int _sector) { return (_endcap == 1) ? _sector - 1 : _sector + 5; }
    int calc_subsector  (int _station, int _chamber ) {                // Why does emulator have csc_ID dependence? - AWB 11.04.16
      if ( _station == 1 ) return (( (_chamber-1) % 6 ) > 2) ? 1 : 2;  // if(station == 1 && id > 3 && id < 7) - AWB 11.04.16
      else if ( _station < 0 || _chamber < 0 ) return -999;            // Also, is this correct for chambers 1 - 36, vs. 0 - 35? - AWB 11.04.16
      else return 0; }                                                // Here, corrected by adding "-1" to "_chamber" - AWB 11.04.16
    float calc_theta_deg (int _theta_int) { return _theta_int * 0.2851562 + 8.5; }
    float calc_theta_rad (int _theta_int) { return (_theta_int * 0.2851562 + 8.5) * (pi / 180); }
    float calc_eta     (float _theta_rad) { return -1 * log( tan( _theta_rad / 2 ) ); }

    // Calculates ring value                                                                                                                     
    int calc_ring(int _station, int _csc_ID, int _strip) {
      if (_station > 1) {
	if      (_csc_ID <  4) return 1;
	else if (_csc_ID < 10) return 2;
	else return -99;
      }
      else if (_station == 1) {
	if      (_csc_ID < 4 && _strip > 127) return 4;
	else if (_csc_ID < 4 && _strip >=  0) return 1;
	else if (_csc_ID > 3 && _csc_ID <  7) return 2;
	else if (_csc_ID > 6 && _csc_ID < 10) return 3;
	else return -99;
      }
      else return -99;
    }
    
    void set_endcap         (int  bits) { endcap        = bits; }
    void set_station        (int  bits) { station       = bits; }
    void set_ring           (int  bits) { ring          = bits; }
    void set_sector         (int  bits) { sector        = bits; }
    void set_sector_GMT     (int  bits) { sector_GMT    = bits; }
    void set_subsector      (int  bits) { subsector     = bits; }
    void set_chamber        (int  bits) { chamber       = bits; }
    void set_layer          (int  bits) { layer         = bits; }
    void set_csc_ID         (int  bits) { csc_ID        = bits; }
    void set_neighbor       (int  bits) { neighbor      = bits; }
    void set_mpc_link       (int  bits) { mpc_link      = bits; }
    void set_wire           (int  bits) { wire          = bits; }
    void set_strip          (int  bits) { strip         = bits; }
    void set_zone_hit       (int  bits) { zone_hit      = bits; }
    void set_track_num      (int  bits) { track_num     = bits; }
    void set_phi_hit        (int  bits) { phi_hit       = bits; }
    void set_phi_z_val      (int  bits) { phi_z_val     = bits; }
    void set_phi_loc_int    (int  bits) { phi_loc_int   = bits; }
    void set_phi_loc_deg    (float val) { phi_loc_deg   = val;  }
    void set_phi_loc_rad    (float val) { phi_loc_rad   = val;  }
    void set_phi_glob_deg   (float val) { (val < 180) ? phi_glob_deg = val : phi_glob_deg = val - 360;  }
    void set_phi_glob_rad   (float val) { (val < pi ) ? phi_glob_rad = val : phi_glob_rad = val - 2*pi; }
    void set_phi_geom_rad   (float val) { phi_geom_rad   = val; }
    void set_theta_int      (int  bits) { theta_int     = bits; }
    void set_theta_loc      (float val) { theta_loc      = val; }
    void set_theta_deg      (float val) { theta_deg      = val; }
    void set_theta_rad      (float val) { theta_rad      = val; }
    void set_eta            (float val) { eta            = val; }
    void set_quality        (int  bits) { quality       = bits; }
    void set_pattern        (int  bits) { pattern       = bits; }
    void set_bend           (int  bits) { bend          = bits; }
    void set_valid          (int  bits) { valid         = bits; }
    void set_sync_err       (int  bits) { sync_err      = bits; }
    void set_bx0            (int  bits) { bx0           = bits; }
    void set_bx             (int  bits) { bx            = bits; }
    void set_is_CSC_hit     (bool expr) { is_CSC_hit    = expr; }
    void set_is_RPC_hit     (bool expr) { is_RPC_hit    = expr; }

    int   Endcap         ()  const { return endcap   ;      }
    int   Station        ()  const { return station  ;      }
    int   Ring           ()  const { return ring     ;      }
    int   Sector         ()  const { return sector   ;      }
    int   Sector_GMT     ()  const { return sector_GMT;     }
    int   Subsector      ()  const { return subsector;      }
    int   Chamber        ()  const { return chamber  ;      }
    int   Layer          ()  const { return layer    ;      }
    int   CSC_ID         ()  const { return csc_ID   ;      }
    int   Neighbor       ()  const { return neighbor ;      }
    int   MPC_link       ()  const { return mpc_link ;      }
    int   Wire           ()  const { return wire     ;      }
    int   Strip          ()  const { return strip    ;      }
    int   Zone_hit       ()  const { return zone_hit ;      }
    int   Track_num      ()  const { return track_num;      }
    int   Phi_hit        ()  const { return phi_hit;        }
    int   Phi_Z_val      ()  const { return phi_z_val;      }
    int   Phi_loc_int    ()  const { return phi_loc_int;    }
    float Phi_loc_deg    ()  const { return phi_loc_deg;    }
    float Phi_loc_rad    ()  const { return phi_loc_rad;    }
    float Phi_glob_deg   ()  const { return phi_glob_deg;   }
    float Phi_glob_rad   ()  const { return phi_glob_rad;   }
    float Phi_geom_rad   ()  const { return phi_geom_rad;   }
    int   Theta_int      ()  const { return theta_int;      }
    float Theta_loc      ()  const { return theta_loc;      }
    float Theta_deg      ()  const { return theta_deg;      }
    float Theta_rad      ()  const { return theta_rad;      }
    float Eta            ()  const { return eta      ;      }
    int   Quality        ()  const { return quality  ;      }
    int   Pattern        ()  const { return pattern  ;      }
    int   Bend           ()  const { return bend     ;      }
    int   Valid          ()  const { return valid    ;      }
    int   Sync_err       ()  const { return sync_err ;      }
    int   BX0            ()  const { return bx0      ;      }
    int   BX             ()  const { return bx       ;      }
    bool  Is_CSC_hit     ()  const { return is_CSC_hit;     }
    bool  Is_RPC_hit     ()  const { return is_RPC_hit;     }


  private:
    
    std::vector<int> zone_contribution; // Filled in emulator from ConvertedHit.ZoneContribution()
    CSCDetId csc_DetId;
    
    int   endcap;       // -1 or 1.  Filled in EMTFHit.cc from CSCDetId, modified
    int   station;      //  1 -  4.  Filled in EMTFHit.cc from CSCDetId
    int   ring;         //  1 -  3.  Filled in EMTFHit.cc from CSCDetId
    int   sector;       //  1 -  6.  Filled in EMTFHit.cc from CSCDetId
    int   sector_GMT;   //  0 - 11.  Filled in EMTFHit.cc using calc_sector_GMT above
    int   subsector;    //  1 -  2.  Filled in EMTFHit.cc or emulator using calc_subsector above
    int   chamber;      //  1 - 36.  Filled in EMTFHit.cc from CSCDetId
    int   layer;        //  ? -  ?.  Filled in BXAnalyzer.h.  How can we access?
    int   csc_ID;       //  1 -  9.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi or emulator from CSCData
    int   neighbor;     //  0 or 1.  Filled in EMTFBlockME.cc 
    int   mpc_link;     //  1 -  3.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    int   wire;         //  1 -  ?.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    int   strip;        //  1 -  ?.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    int   zone_hit;     //  4 - 118. Filled in emulator from ConvertedHit.Zhit()
    int   track_num;    //  ? -  ?.  Filled in emulator from CSCData 
    int   phi_hit;      //  1 - 42.  Filled in emulator from ConvertedHit.Ph_hit()
    int   phi_z_val;    //  1 -  6.  Filled in emulator from ConvertedHit.Phzvl()
    int   phi_loc_int;  //  ? -  ?.  Filled in emulator from ConvertedHit.Phi()
    float phi_loc_deg;  //  ? -  ?.  Filled in emulator, calculated from phi_loc_int with GetPackedPhi
    float phi_loc_rad;  //  ? -  ?.  Filled in emulator, calculated from phi_loc_int with GetPackedPhi
    float phi_glob_deg; //  +/-180.  Filled in emulator, calculated from phi_loc_int with GetPackedPhi
    float phi_glob_rad; //  +/- pi.  Filled in emulator, calculated from phi_loc_int with GetPackedPhi
    float phi_geom_rad; //  The global phi value returned by L1Trigger/L1TMuon/interface/deprecate/GeometryTranslator.h.  Not yet filled - AWB 06.04.16
    int   theta_int;    //  ? -  ?.  Filled in emulator from ConvertedHit.Theta()
    float theta_loc;    //  Some bizzare local definition of theta.  Not yet filled - AWB 06.04.16
    float theta_deg;    // 10 - 45.  Filled in emulator from calc_theta_deg above
    float theta_rad;    // .2 - .8.  Filled in emulator from calc_theta_rad above
    float eta;          // +/- 2.5.  Filled in emulator from calc_eta above
    int   quality;      //  0 - 15.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    int   pattern;      //  0 - 10.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    int   bend;         //  0 or 1.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    int   valid;        //  0 or 1.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    int   sync_err;     //  0 or 1.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    int   bx0;          //  1-3600.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    int   bx;           //  3 -  9.  Filled in EMTFHit.cc from CSCCorrelatedLCTDigi
    bool  is_CSC_hit;   //  0 or 1.  Filled in EMTFHit.cc
    bool  is_RPC_hit;   //  0 or 1.  Filled in EMTFHit.cc

  }; // End of class EMTFHit
  
  // Define a vector of EMTFHit
  typedef std::vector<EMTFHit> EMTFHitCollection;
  
} // End of namespace l1t

#endif /* define __l1t_EMTFHit_h__ */
