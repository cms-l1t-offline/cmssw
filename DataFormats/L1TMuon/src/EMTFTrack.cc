#include "DataFormats/L1TMuon/interface/EMTFTrack.h"

namespace l1t {


  void EMTFTrack::ImportSP( const emtf::SP _SP, int _sector) {

    set_sector       ( _sector );
    set_mode         ( _SP.Mode() );         
    set_quality      ( _SP.Quality_GMT() );      
    set_first_bx     ( _SP.TBIN() - 3 );     
    set_pt_GMT       ( _SP.Pt_GMT() );       
    set_pt_LUT       ( _SP.Pt_LUT_addr() );
    set_eta_GMT      ( _SP.Eta_GMT() );      
    set_phi_loc_int  ( _SP.Phi_full() );  
    set_phi_GMT      ( _SP.Phi_GMT() );      

    set_pt           ( calc_pt( pt_GMT ) );
    set_eta          ( calc_eta( eta_GMT ) );
    set_theta_deg    ( calc_theta_deg( eta ) );
    set_theta_rad    ( calc_theta_rad( eta ) );
    set_phi_loc_deg  ( calc_phi_loc_deg( phi_loc_int ) );
    set_phi_loc_rad  ( calc_phi_loc_rad( phi_loc_int ) );
    set_phi_glob_deg ( calc_phi_glob_deg( phi_loc_deg, _sector ) );
    set_phi_glob_rad ( calc_phi_glob_rad( phi_loc_rad, _sector ) );

    // Not yet filled - AWB 21.04.16
    // set_type         ( _SP.() );         
    // set_rank         ( _SP.() );         
    // set_layer        ( _SP.() );        
    // set_straightness ( _SP.() ); 
    // set_strip        ( _SP.() );        
    // set_second_bx    ( _SP.() );    
    // set_pt_XML       ( _SP.() );       
    // set_theta_int    ( _SP.() );    
    // set_charge       ( _SP.() );       
    // set_charge_GMT   ( _SP.() );   
    // set_isGMT        ( _SP.() );        

  } // End EMTFHit::ImportSP


  // Unpacks pT LUT address into dPhi, dTheta, CLCT, FR, eta, and mode   
  // Based on L1Trigger/L1TMuonEndCap/interface/PtAssignment.h
  // "Mode" here is the true mode, not the inverted mode used in PtAssignment.h
  void EMTFTrack::Import_pT_LUT(int _mode, unsigned long _address) {
    if     (_mode == 12) { // mode_inv == 3
      set_dPhi_12     ( ( _address >> (0) )   & ( (1 << 9) - 1) );
      set_dPhi_12 (dPhi_12 * (( ( _address >> (0+9) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_dTheta_12   ( ( _address >> (0+9+1) ) & ( (1 << 3) - 1) );
      set_clct_1      ( ( _address >> (0+9+1+3) ) & ( (1 << 2) - 1) );
      set_clct_1 (clct_1   * ( ( ( _address >> (0+9+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_clct_2      ( ( _address >> (0+9+1+3+2+1) ) & ( (1 << 2) - 1) );
      set_clct_2 (clct_2   * ( ( ( _address >> (0+9+1+3+2+1+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_fr_1        ( ( _address >> (0+9+1+3+2+1+2+1) ) & ( (1 << 1) - 1) );
      set_fr_2        ( ( _address >> (0+9+1+3+2+1+2+1+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+9+1+3+2+1+2+1+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+9+1+3+2+1+2+1+1+1+5) ) & ( (1 << 4) - 1) );
    }
    else if (_mode == 10) { // mode_inv == 5
      set_dPhi_13     ( ( _address >> (0) )   & ( (1 << 9) - 1) );
      set_dPhi_13 (dPhi_13 * (( ( _address >> (0+9) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_dTheta_13   ( ( _address >> (0+9+1) ) & ( (1 << 3) - 1) );
      set_clct_1      ( ( _address >> (0+9+1+3) ) & ( (1 << 2) - 1) );
      set_clct_1 (clct_1   * ( ( ( _address >> (0+9+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_clct_3      ( ( _address >> (0+9+1+3+2+1) ) & ( (1 << 2) - 1) );
      set_clct_3 (clct_3   * ( ( ( _address >> (0+9+1+3+2+1+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_fr_1        ( ( _address >> (0+9+1+3+2+1+2+1) ) & ( (1 << 1) - 1) );
      set_fr_3        ( ( _address >> (0+9+1+3+2+1+2+1+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+9+1+3+2+1+2+1+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+9+1+3+2+1+2+1+1+1+5) ) & ( (1 << 4) - 1) );
    }
    else if (_mode == 9) { // mode_inv == 9
      set_dPhi_14     ( ( _address >> (0) )   & ( (1 << 9) - 1) );
      set_dPhi_14 (dPhi_14 * (( ( _address >> (0+9) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_dTheta_14   ( ( _address >> (0+9+1) ) & ( (1 << 3) - 1) );
      set_clct_1      ( ( _address >> (0+9+1+3) ) & ( (1 << 2) - 1) );
      set_clct_1 (clct_1   * ( ( ( _address >> (0+9+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_clct_4      ( ( _address >> (0+9+1+3+2+1) ) & ( (1 << 2) - 1) );
      set_clct_4 (clct_4   * ( ( ( _address >> (0+9+1+3+2+1+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_fr_1        ( ( _address >> (0+9+1+3+2+1+2+1) ) & ( (1 << 1) - 1) );
      set_fr_4        ( ( _address >> (0+9+1+3+2+1+2+1+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+9+1+3+2+1+2+1+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+9+1+3+2+1+2+1+1+1+5) ) & ( (1 << 4) - 1) );
    }
    else if (_mode == 6) { // mode_inv = 6
      set_dPhi_23     ( ( _address >> (0) )   & ( (1 << 9) - 1) );
      set_dPhi_23 (dPhi_23 * (( ( _address >> (0+9) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_dTheta_23   ( ( _address >> (0+9+1) ) & ( (1 << 3) - 1) );
      set_clct_2      ( ( _address >> (0+9+1+3) ) & ( (1 << 2) - 1) );
      set_clct_2 (clct_2   * ( ( ( _address >> (0+9+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_clct_3      ( ( _address >> (0+9+1+3+2+1) ) & ( (1 << 2) - 1) );
      set_clct_3 (clct_3   * ( ( ( _address >> (0+9+1+3+2+1+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_fr_2        ( ( _address >> (0+9+1+3+2+1+2+1) ) & ( (1 << 1) - 1) );
      set_fr_3        ( ( _address >> (0+9+1+3+2+1+2+1+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+9+1+3+2+1+2+1+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+9+1+3+2+1+2+1+1+1+5) ) & ( (1 << 4) - 1) );
    }
    else if (_mode == 5) { // mode_inv == 10
      set_dPhi_24     ( ( _address >> (0) )   & ( (1 << 9) - 1) );
      set_dPhi_24 (dPhi_24 * (( ( _address >> (0+9) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_dTheta_24   ( ( _address >> (0+9+1) ) & ( (1 << 3) - 1) );
      set_clct_2      ( ( _address >> (0+9+1+3) ) & ( (1 << 2) - 1) );
      set_clct_2 (clct_2   * ( ( ( _address >> (0+9+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_clct_4      ( ( _address >> (0+9+1+3+2+1) ) & ( (1 << 2) - 1) );
      set_clct_4 (clct_4   * ( ( ( _address >> (0+9+1+3+2+1+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_fr_2        ( ( _address >> (0+9+1+3+2+1+2+1) ) & ( (1 << 1) - 1) );
      set_fr_4        ( ( _address >> (0+9+1+3+2+1+2+1+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+9+1+3+2+1+2+1+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+9+1+3+2+1+2+1+1+1+5) ) & ( (1 << 4) - 1) );
    }
    else if (_mode == 3) { // mode_inv == 12
      set_dPhi_34     ( ( _address >> (0) )   & ( (1 << 9) - 1) );
      set_dPhi_34 (dPhi_34 * (( ( _address >> (0+9) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_dTheta_34   ( ( _address >> (0+9+1) ) & ( (1 << 3) - 1) );
      set_clct_3      ( ( _address >> (0+9+1+3) ) & ( (1 << 2) - 1) );
      set_clct_3 (clct_3   * ( ( ( _address >> (0+9+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_clct_4      ( ( _address >> (0+9+1+3+2+1) ) & ( (1 << 2) - 1) );
      set_clct_4 (clct_4   * ( ( ( _address >> (0+9+1+3+2+1+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_fr_3        ( ( _address >> (0+9+1+3+2+1+2+1) ) & ( (1 << 1) - 1) );
      set_fr_4        ( ( _address >> (0+9+1+3+2+1+2+1+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+9+1+3+2+1+2+1+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+9+1+3+2+1+2+1+1+1+5) ) & ( (1 << 4) - 1) );
    }      
    else if (_mode == 14) { // mode_inv == 7
      set_dPhi_12     ( ( _address >> (0) )     & ( (1 << 7) - 1) );
      set_dPhi_23     ( ( _address >> (0+7) )   & ( (1 << 5) - 1) );
      set_dPhi_12 (dPhi_12 * (( ( _address >> (0+7+5) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_dPhi_23 (dPhi_23 * (( ( _address >> (0+7+5+1) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_dTheta_13   ( ( _address >> (0+7+5+1+1) ) & ( (1 << 3) - 1) );
      set_clct_1      ( ( _address >> (0+7+5+1+1+3) ) & ( (1 << 2) - 1) );
      set_clct_1 (clct_1   * ( ( ( _address >> (0+7+5+1+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_fr_1        ( ( _address >> (0+7+5+1+1+3+2+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+7+5+1+1+3+2+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+7+5+1+1+3+2+1+1+5) ) & ( (1 << 4) - 1) );
    }
    else if (_mode == 13) { // mode_inv == 11
      set_dPhi_12     ( ( _address >> (0) )     & ( (1 << 7) - 1) );
      set_dPhi_24     ( ( _address >> (0+7) )   & ( (1 << 5) - 1) );
      set_dPhi_12 (dPhi_12 * (( ( _address >> (0+7+5) ) & ( (1 << 1) - 1 ) ) == 0 ? -1 : 1) );
      set_dPhi_24 (dPhi_24 * (( ( _address >> (0+7+5+1) ) & ( (1 << 1) - 1 ) ) == 0 ? -1 : 1) );
      set_dTheta_14   ( ( _address >> (0+7+5+1+1) ) & ( (1 << 3) - 1) );
      set_clct_1      ( ( _address >> (0+7+5+1+1+3) ) & ( (1 << 2) - 1) );
      set_clct_1 (clct_1   * ( ( ( _address >> (0+7+5+1+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_fr_1        ( ( _address >> (0+7+5+1+1+3+2+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+7+5+1+1+3+2+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+7+5+1+1+3+2+1+1+5) ) & ( (1 << 4) - 1) );
    }
    else if (_mode == 11) {
      set_dPhi_13     ( ( _address >> (0) )     & ( (1 << 7) - 1) );
      set_dPhi_34     ( ( _address >> (0+7) )   & ( (1 << 5) - 1) );
      set_dPhi_13 (dPhi_13 * (( ( _address >> (0+7+5) ) & ( (1 << 1) - 1 ) ) == 0 ? -1 : 1) );
      set_dPhi_34 (dPhi_34 * (( ( _address >> (0+7+5+1) ) & ( (1 << 1) - 1 ) ) == 0 ? -1 : 1) );
      set_dTheta_14   ( ( _address >> (0+7+5+1+1) ) & ( (1 << 3) - 1) );
      set_clct_1      ( ( _address >> (0+7+5+1+1+3) ) & ( (1 << 2) - 1) );
      set_clct_1 (clct_1   * ( ( ( _address >> (0+7+5+1+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_fr_1        ( ( _address >> (0+7+5+1+1+3+2+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+7+5+1+1+3+2+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+7+5+1+1+3+2+1+1+5) ) & ( (1 << 4) - 1) );
    }
    else if (_mode == 7) { // mode_inv == 14
      set_dPhi_23     ( ( _address >> (0) )     & ( (1 << 7) - 1) );
      set_dPhi_34     ( ( _address >> (0+7) )   & ( (1 << 6) - 1) );
      set_dPhi_23 (dPhi_23 * (( ( _address >> (0+7+6) ) & ( (1 << 1) - 1 ) ) == 0 ? -1 : 1) );
      set_dPhi_34 (dPhi_34 * (( ( _address >> (0+7+6+1) ) & ( (1 << 1) - 1 ) ) == 0 ? -1 : 1) );
      set_dTheta_24   ( ( _address >> (0+7+6+1+1) ) & ( (1 << 3) - 1) );
      set_clct_2      ( ( _address >> (0+7+5+1+1+3) ) & ( (1 << 2) - 1) );
      set_clct_2 (clct_2   * ( ( ( _address >> (0+7+6+1+1+3+2) ) & ( (1 << 1) - 1) ) == 0 ? -1 : 1) );
      set_eta_LUT     ( ( _address >> (0+7+6+1+1+3+2+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+7+6+1+1+3+2+1+5) ) & ( (1 << 4) - 1) );
    }
    else if (_mode == 15) { // mode_inv == 15
      set_dPhi_12     ( ( _address >> (0) )     & ( (1 << 7) - 1) );
      set_dPhi_23     ( ( _address >> (0+7) )   & ( (1 << 5) - 1) );
      set_dPhi_34     ( ( _address >> (0+7+5) ) & ( (1 << 6) - 1) );
      set_dPhi_23 (dPhi_23 * (( ( _address >> (0+7+5+6) ) & ( (1 << 1) - 1 ) ) == 0 ? -1 : 1) );
      set_dPhi_34 (dPhi_34 * (( ( _address >> (0+7+5+6+1) ) & ( (1 << 1) - 1 ) ) == 0 ? -1 : 1) );
      set_fr_1        ( ( _address >> (0+7+5+6+1+1) ) & ( (1 << 1) - 1) );
      set_eta_LUT     ( ( _address >> (0+7+5+6+1+1+1) ) & ( (1 << 5) - 1) );
      set_mode_LUT    ( ( _address >> (0+7+5+6+1+1+1+5) ) & ( (1 << 4) - 1) );
    }

  } // End function: std::vector<int> convert_EMTF_pT_LUT
    
} // End namespace l1t
