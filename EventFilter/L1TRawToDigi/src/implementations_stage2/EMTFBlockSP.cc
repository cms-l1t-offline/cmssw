// Code to unpack the "SP Output Data Record"

#include "EventFilter/L1TRawToDigi/interface/Unpacker.h"
#include "EMTFCollections.h"
#include "EMTFUnpackerTools.h"

// This is the "header" - no EMTFBlockSP.h file is needed
namespace l1t {
  namespace stage2 {
    namespace emtf {
      
      class SPBlockUnpacker : public Unpacker { // "SPBlockUnpacker" inherits from "Unpacker"
      public:
	virtual int  checkFormat(const Block& block);
	virtual bool unpack(const Block& block, UnpackerCollections *coll) override; // Apparently it's always good to use override in C++
	// virtual bool packBlock(const Block& block, UnpackerCollections *coll) override;
      };
      

      // class SPBlockPacker : public Packer { // "SPBlockPacker" inherits from "Packer"
      // public:
      // 	virtual bool unpack(const Block& block, UnpackerCollections *coll) override; // Apparently it's always good to use override in C++
      // };
      
    }
  }
}

namespace l1t {
  namespace stage2 {
    namespace emtf {

      int SPBlockUnpacker::checkFormat(const Block& block) {

	auto payload = block.payload();
	int errors = 0;

	//Check the number of 16-bit words                                                                                                                                    
	if(payload.size() != 8) { errors += 1; edm::LogError("L1T|EMTF") << "Payload size in 'SP Output Data Record' is different than expected"; }

	//Check that each word is 16 bits                                                                                                                                     
	if(GetHexBits(payload[0], 16, 31) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Payload[0] has more than 16 bits in 'SP Output Data Record'"; }
	if(GetHexBits(payload[1], 16, 31) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Payload[1] has more than 16 bits in 'SP Output Data Record'"; }
	if(GetHexBits(payload[2], 16, 31) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Payload[2] has more than 16 bits in 'SP Output Data Record'"; }
	if(GetHexBits(payload[3], 16, 31) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Payload[3] has more than 16 bits in 'SP Output Data Record'"; }
	if(GetHexBits(payload[4], 16, 31) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Payload[4] has more than 16 bits in 'SP Output Data Record'"; }
	if(GetHexBits(payload[5], 16, 31) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Payload[5] has more than 16 bits in 'SP Output Data Record'"; }
	if(GetHexBits(payload[6], 16, 31) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Payload[6] has more than 16 bits in 'SP Output Data Record'"; }
	if(GetHexBits(payload[7], 16, 31) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Payload[7] has more than 16 bits in 'SP Output Data Record'"; }

	uint16_t SP1a = payload[0];
	uint16_t SP1b = payload[1];
	uint16_t SP1c = payload[2];
	uint16_t SP1d = payload[3];
	uint16_t SP2a = payload[4];
	uint16_t SP2b = payload[5];
	uint16_t SP2c = payload[6];
	uint16_t SP2d = payload[7];
      
	//Check Format                                                                                                                                                        
	if(GetHexBits(SP1a, 15, 15) != 1) { errors += 1; edm::LogError("L1T|EMTF") << "Format identifier bits in SP1a are incorrect"; }
	if(GetHexBits(SP1b, 15, 15) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Format identifier bits in SP1b are incorrect"; }
	// if(GetHexBits(SP1b, 8, 11)  != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Format identifier bits in SP1b are incorrect"; } // Until 01.04.16
	if(GetHexBits(SP1c, 15, 15) != 1) { errors += 1; edm::LogError("L1T|EMTF") << "Format identifier bits in SP1c are incorrect"; }
	if(GetHexBits(SP1d, 15, 15) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Format identifier bits in SP1d are incorrect"; }
	if(GetHexBits(SP2a, 15, 15) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Format identifier bits in SP2a are incorrect"; }
	if(GetHexBits(SP2b, 15, 15) != 1) { errors += 1; edm::LogError("L1T|EMTF") << "Format identifier bits in SP2b are incorrect"; }
	if(GetHexBits(SP2c, 15, 15) != 1) { errors += 1; edm::LogError("L1T|EMTF") << "Format identifier bits in SP2c are incorrect"; }
	if(GetHexBits(SP2d, 15, 15) != 0) { errors += 1; edm::LogError("L1T|EMTF") << "Format identifier bits in SP2d are incorrect"; }

	return errors;

      }


      // Converts CSC_ID, sector, subsector, and neighbor                                                                                          
      std::vector<int> convert_SP_location(int _csc_ID, int _sector, int _subsector, int _station) {
        int new_sector = _sector;
        if (_station == 1) {
          if      (_csc_ID <  0) { int arr[] = {-99, -99, -99, -99}; std::vector<int> vec(arr, arr+4); return vec; }
          else if (_csc_ID == 0) { int arr[] = { -1,  -1,  -1,  -1}; std::vector<int> vec(arr, arr+4); return vec; }
          else if (_csc_ID <= 9) { int arr[] = {_csc_ID, new_sector, _subsector, 0}; std::vector<int> vec(arr, arr+4); return vec; }
          else new_sector = (_sector != 1) ? _sector-1 : 6;
	  
          if      (_csc_ID == 10) { int arr[] = {3, new_sector, 2, 1}; std::vector<int> vec(arr, arr+4); return vec; }
          else if (_csc_ID == 11) { int arr[] = {6, new_sector, 2, 1}; std::vector<int> vec(arr, arr+4); return vec; }
          else if (_csc_ID == 12) { int arr[] = {9, new_sector, 2, 1}; std::vector<int> vec(arr, arr+4); return vec; }
          else { int arr[] = {-99, -99, -99, -99}; std::vector<int> vec(arr, arr+4); return vec; }
        }
        else if (_station == 2 || _station == 3 || _station == 4) {
          if      (_csc_ID <  0) { int arr[] = {-99, -99, -99, -99}; std::vector<int> vec(arr, arr+4); return vec; }
          else if (_csc_ID == 0) { int arr[] = { -1,  -1,  -1,  -1}; std::vector<int> vec(arr, arr+4); return vec; }
          else if (_csc_ID <= 9) { int arr[] = {_csc_ID, new_sector, -1, 0}; std::vector<int> vec(arr, arr+4); return vec; }
          else new_sector = (_sector != 1) ? _sector-1 : 6;
	  
          if      (_csc_ID == 10) { int arr[] = {3, new_sector, -1, 1}; std::vector<int> vec(arr, arr+4); return vec; }
          else if (_csc_ID == 11) { int arr[] = {6, new_sector, -1, 1}; std::vector<int> vec(arr, arr+4); return vec; }
          else { int arr[] = {-99, -99, -99, -99}; std::vector<int> vec(arr, arr+4); return vec; }
        }
        else { int arr[] = {-99, -99, -99, -99}; std::vector<int> vec(arr, arr+4); return vec; }
      }


      bool SPBlockUnpacker::unpack(const Block& block, UnpackerCollections *coll) {
	
	// std::cout << "Inside EMTFBlockSP.cc: unpack" << std::endl;
	// LogDebug("L1T|EMTF") << "Inside EMTFBlockSP.cc: unpack"; // Why doesn't this work? - AWB 09.04.16
	
	// Get the payload for this block, made up of 16-bit words (0xffff)
	// Format defined in MTF7Payload::getBlock() in src/Block.cc
	// payload[0] = bits 0-15, payload[1] = 16-31, payload[3] = 32-47, etc.
	auto payload = block.payload();

	// Check Format of Payload
	l1t::emtf::SP SP_;
	for (int err = 0; err < checkFormat(block); err++) SP_.add_format_error();

        // Assign payload to 16-bit words
        uint16_t SP1a = payload[0];	
        uint16_t SP1b = payload[1];	
        uint16_t SP1c = payload[2];	
        uint16_t SP1d = payload[3];	
        uint16_t SP2a = payload[4];	
        uint16_t SP2b = payload[5];	
        uint16_t SP2c = payload[6];	
        uint16_t SP2d = payload[7];	

	// res is a pointer to a collection of EMTFOutput class objects
	// There is one EMTFOutput for each MTF7 (60 deg. sector) in the event
	EMTFOutputCollection* res;
	res = static_cast<EMTFCollections*>(coll)->getEMTFOutputs();
	int iOut = res->size() - 1;
	std::vector<int> conv_vals_SP;
	std::vector<int> conv_vals_pT_LUT;

	EMTFTrackCollection* res_track;
        res_track = static_cast<EMTFCollections*>(coll)->getEMTFTracks();
        EMTFTrack Track_;

	RegionalMuonCandBxCollection* res_cand;
	res_cand = static_cast<EMTFCollections*>(coll)->getRegionalMuonCands();
	RegionalMuonCand mu_;
	
	// if (SP_.Format_Errors() > 0) goto write; // Temporarily disable for DQM operation - AWB 09.04.16

	///////////////////////////////////
	// Unpack the SP Output Data Record
	///////////////////////////////////

	// SP_.set_phi_full        ( GetHexBits(SP1a,  0, 11) ); // Until 01.04.16
	SP_.set_phi_full     ( GetHexBits(SP1a,  0, 12) ); // After 01.04.16
	// SP_.set_vc           ( GetHexBits(SP1a, 12, 12) ); // Until 01.04.16
	SP_.set_c            ( GetHexBits(SP1a, 13, 13) );
	SP_.set_hl           ( GetHexBits(SP1a, 14, 14) );

	SP_.set_phi_GMT      ( TwosCompl(8, GetHexBits(SP1b, 0, 7)) );
	mu_.setHwPhi         ( TwosCompl(8, GetHexBits(SP1b, 0, 7)) );
	SP_.set_quality_GMT  ( GetHexBits(SP1b,  8, 11) ); // After 01.04.16
	mu_.setHwQual        ( GetHexBits(SP1b,  8, 11) );
	SP_.set_bc0          ( GetHexBits(SP1b, 12, 12) );
	SP_.set_se           ( GetHexBits(SP1b, 13, 13) );
	/// SP_.set_vt           ( GetHexBits(SP1b, 14, 14) ); // Until 01.04.16
	SP_.set_vc           ( GetHexBits(SP1b, 14, 14) ); // After 01.04.16

	SP_.set_eta_GMT      ( TwosCompl(9, GetHexBits(SP1c, 0, 8)) );
	mu_.setHwEta         ( TwosCompl(9, GetHexBits(SP1c, 0, 8)) );
	// SP_.set_quality_GMT  ( GetHexBits(SP1c,  9, 12) ); // Until 01.04.16
	SP_.set_mode         ( GetHexBits(SP1c,  9, 12) ); // After 01.04.16
	SP_.set_bx           ( GetHexBits(SP1c, 13, 14) );

	SP_.set_pt_GMT       ( GetHexBits(SP1d,  0,   8) );
	mu_.setHwPt          ( GetHexBits(SP1d,  0,   8) );
	SP_.set_me1_stub_num ( GetHexBits(SP1d,  9,   9) );
	SP_.set_me1_CSC_ID   ( GetHexBits(SP1d, 10,  13) );
	SP_.set_me1_subsector( GetHexBits(SP1d, 14,  14) );

	SP_.set_me2_stub_num ( GetHexBits(SP2a,  0, 0 ) );
	SP_.set_me2_CSC_ID   ( GetHexBits(SP2a,  1, 4 ) );
	SP_.set_me3_stub_num ( GetHexBits(SP2a,  5, 5 ) );
	SP_.set_me3_CSC_ID   ( GetHexBits(SP2a,  6, 9 ) );
	SP_.set_me4_stub_num ( GetHexBits(SP2a, 10, 10) );
	SP_.set_me4_CSC_ID   ( GetHexBits(SP2a, 11, 14) );

	SP_.set_me1_delay    ( GetHexBits(SP2b,  0,  2) );
	SP_.set_me2_delay    ( GetHexBits(SP2b,  3,  5) );
	SP_.set_me3_delay    ( GetHexBits(SP2b,  6,  8) );
	SP_.set_me4_delay    ( GetHexBits(SP2b,  9, 11) );
	SP_.set_tbin         ( GetHexBits(SP2b, 12, 14) );

	SP_.set_pt_LUT_addr  ( GetHexBits(SP2c,  0, 14, SP2d,  0, 14) );

	// SP_.set_dataword     ( uint64_t dataword );
	// mu_.set_dataword     ( uint64_t dataword );


	Track_.ImportSP( SP_, (res->at(iOut)).PtrEventHeader()->Sector() );
	Track_.Import_pT_LUT( Track_.Mode(), Track_.Pt_LUT() );
	Track_.set_endcap       ( ((res->at(iOut)).PtrEventHeader()->Endcap() == 1) ? 1 : -1 );
	Track_.set_sector_GMT   ( (res->at(iOut)).PtrEventHeader()->SP_TS() );
	
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
	

        // After 01.04.16
	conv_vals_SP = convert_SP_location( SP_.ME1_CSC_ID(), (res->at(iOut)).PtrEventHeader()->Sector(), SP_.ME1_subsector(), 1 );
        Track_.set_me1_CSC_ID    ( conv_vals_SP.at(0) );
        Track_.set_me1_sector    ( conv_vals_SP.at(1) );
        Track_.set_me1_subsector ( conv_vals_SP.at(2) );
        Track_.set_me1_neighbor  ( conv_vals_SP.at(3) );

	conv_vals_SP = convert_SP_location( SP_.ME2_CSC_ID(), (res->at(iOut)).PtrEventHeader()->Sector(), -99, 2 );
        Track_.set_me2_CSC_ID    ( conv_vals_SP.at(0) );
        Track_.set_me2_sector    ( conv_vals_SP.at(1) );
        Track_.set_me2_neighbor  ( conv_vals_SP.at(3) );

	conv_vals_SP = convert_SP_location( SP_.ME3_CSC_ID(), (res->at(iOut)).PtrEventHeader()->Sector(), -99, 3 );
        Track_.set_me3_CSC_ID    ( conv_vals_SP.at(0) );
        Track_.set_me3_sector    ( conv_vals_SP.at(1) );
        Track_.set_me3_neighbor  ( conv_vals_SP.at(3) );

	conv_vals_SP = convert_SP_location( SP_.ME4_CSC_ID(), (res->at(iOut)).PtrEventHeader()->Sector(), -99, 4 );
        Track_.set_me4_CSC_ID    ( conv_vals_SP.at(0) );
        Track_.set_me4_sector    ( conv_vals_SP.at(1) );
        Track_.set_me4_neighbor  ( conv_vals_SP.at(3) );

	// write: // Temporarily disable for DQM operation - AWB 09.04.16

	(res->at(iOut)).push_SP(SP_);

	res_track->push_back( Track_ );

	// TBIN_num can range from 0 through 7, i.e. BX = -3 through +4. - AWB 04.04.16
	res_cand->setBXRange(-3, 4);
	res_cand->push_back(SP_.TBIN() - 3, mu_);

	// Finished with unpacking one SP Output Data Record
	return true;
	
      } // End bool SPBlockUnpacker::unpack

      // bool SPBlockPacker::pack(const Block& block, UnpackerCollections *coll) {
      // 	std::cout << "Inside SPBlockPacker::pack" << std::endl;
      // 	return true;
      // } // End bool SPBlockPacker::pack

    } // End namespace emtf
  } // End namespace stage2
} // End namespace l1t

DEFINE_L1T_UNPACKER(l1t::stage2::emtf::SPBlockUnpacker);
// DEFINE_L1T_PACKER(l1t::stage2::SPBlockPacker);
