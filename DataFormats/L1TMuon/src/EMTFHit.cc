#include "DataFormats/L1TMuon/interface/EMTFHit.h"

namespace l1t {

  // Based on L1Trigger/L1TMuon/src/MuonTriggerPrimitive.cc
  // TriggerPrimitive::TriggerPrimitive(const CSCDetId& detid, const CSCCorrelatedLCTDigi& digi)
  void EMTFHit::ImportCSCDetId( const CSCDetId& _detId) {

    SetCSCDetId ( _detId ); 
    // It appears the following function *actually does literally nothing* - AWB 17.03.16
    // calculateCSCGlobalSector(detid,_globalsector,_subsector);

    // Based on L1Trigger/L1TMuonEndCap/interface/PrimitiveConverter.h
    set_endcap  ( (_detId.endcap() == 2) ? -1 : _detId.endcap() ); // Convert from {+,-} = {1,2} to {1,-1}
    set_station ( _detId.station()       );
    set_sector  ( _detId.triggerSector() );
    set_ring    ( _detId.ring()          );
    set_chamber ( _detId.chamber()       );

    set_sector_GMT ( calc_sector_GMT( Endcap(), Sector() ) );
    set_is_CSC_hit ( true  );
    set_is_RPC_hit ( false );

  } // End EMTFHit::ImportCSCDetId

  // Based on L1Trigger/L1TMuon/src/MuonTriggerPrimitive.cc
  // TriggerPrimitive::TriggerPrimitive(const CSCDetId& detid, const CSCCorrelatedLCTDigi& digi)
  // This is what gets filled when "getCSCData()" is called in
  // L1Trigger/L1TMuonEndCap/interface/PrimitiveConverter.h
  void EMTFHit::ImportCSCCorrelatedLCTDigi( const CSCCorrelatedLCTDigi& _digi) {

    set_track_num ( _digi.getTrknmb()  );
    set_valid     ( _digi.isValid()    );
    set_quality   ( _digi.getQuality() );
    set_wire      ( _digi.getKeyWG()   );
    set_strip     ( _digi.getStrip()   );
    set_pattern   ( _digi.getPattern() );
    set_bend      ( _digi.getBend()    );
    set_bx        ( _digi.getBX() - 6  ); // Standard for csctfDigis in data, simCscTriggerPrimitiveDigis in MC
    set_mpc_link  ( _digi.getMPCLink() );
    set_bx0       ( _digi.getBX0()     );
    set_sync_err  ( _digi.getSyncErr() );
    set_csc_ID    ( _digi.getCSCID()   );

    set_subsector ( calc_subsector( Station(), Chamber() ) ); 

  } // End EMTFHit::ImportCSCCorrelatedLCTDigi

  void EMTFHit::ImportME( const emtf::ME _ME) {

    set_wire       ( _ME.Wire() );
    set_strip      ( _ME.Strip() );
    set_quality    ( _ME.Quality() );
    set_pattern    ( _ME.CLCT_pattern() );
    set_bend       ( (_ME.LR() == 1) ? 1 : -1 );
    set_valid      ( _ME.VP() );
    set_sync_err   ( _ME.SE() );
    set_bx         ( _ME.TBIN() - 3 );
    set_is_CSC_hit ( true  );
    set_is_RPC_hit ( false );

    // Not yet filled - AWB 21.04.16
    // set_chamber();
    // set_layer();

  } // End EMTFHit::ImportME

} // End namespace l1t
