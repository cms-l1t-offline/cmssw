#include "L1Trigger/L1TMuon/src/Phase2/L1TDisplacedMuonPtAssignment.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"

#include "L1Trigger/L1TMuon/src/Phase2/EndcapTriggerPtAssignmentHelper.h"
#include "L1Trigger/L1TMuon/src/Phase2/BarrelTriggerPtAssignmentHelper.h"

#include "iostream"

using namespace L1TMuon;
using namespace EndcapTriggerPtAssignmentHelper;

L1TDisplacedMuonPtAssignment::L1TDisplacedMuonPtAssignment(const edm::ParameterSet& iConfig)
{
  // assignment_.reset(new L1TMuon::L1TDisplacedMuonPtAssignment(iConfig));

  // std::unique_ptr<EndcapTriggerPtAssignmentHelper> endcapHelper_;
  // std::unique_ptr<BarrelTriggerPtAssignmentHelper> barrelHelper_;
  //
  // FIXME
  // steps to finish pt assignment in endcap
  // step1: parameter assignment from config 
  // step2: get stubs, GEMPads and ME0 segment 
  // step3: get optimized CSC positioin by fitting comparator digis ?? how to do that at firmware level as L1 firmware can not talk with CFEB?
  // step4: displaced pt assignment: low eta, 1.2-1.6, position only
  // high eta: 1.6-2.1: position, direction, hybrid, and GE11, GE21 are used
  // very high eta: 2.1-2.4: position, direction, hybrid, and ME0, GE21 are used
  
  float muoneta = muon_.eta();
  if (fabs(muoneta) > 1.2)//Endcap
  {

    convertStubsIntoGlobalpoints();
    if (fabs(muoneta) > 1.6)
	convertGEMPadsIntoGlobalpoints();
    if (fabs(muoneta) > 2.1)
	convertME0SegIntoGlobalpoints();

    //pt assignment 
    //FIXME: how to make sure necessary segment available before pt assignment 
    if (hasCSC_[0] and hasCSC_[1] and hasCSC_[2]){
	calculatePositionPtEndcap();
	if (fabs(muoneta) > 1.6 and fabs(muoneta) < 2.1 and hasGEM_[0] and hasGEM_[1]){
	    calculateDirectionPtEndcapMedium();
	    calculateHybridPtEndcapMedium();
	}else if (fabs(muoneta) >= 2.1 and hasGEM_[1] and hasME0_){
	    calculateDirectionPtEndcapHigh();
	    calculateHybridPtEndcapHigh();
	}
    }
    
  }
}


L1TDisplacedMuonPtAssignment::~L1TDisplacedMuonPtAssignment()
{
}

void L1TDisplacedMuonPtAssignment::initVariables(){
    meRing = -1;
}

void L1TDisplacedMuonPtAssignment::convertStubsIntoGlobalpoints()
{
    int keystation = 2;

    for (auto idlct : lcts_){
	CSCDetId chid(idlct.first);
	hasCSC_[chid.station()-1] = true;
	gp_ME[chid.station()-1] = GeometryHelpers::globalPositionOfCSCLCT(csc_g, idlct.second, chid);
//FIXME here: gp_st_layer3 was from fitting comparator digis in old code but NOW it is just from stub
	gp_st_layer3[chid.station()-1] = gp_ME[chid.station()-1];
	if (chid.station() == keystation)
	    meRing = chid.ring();
    }

}
void L1TDisplacedMuonPtAssignment::convertGEMPadsIntoGlobalpoints(){
    for (auto idpad : pads_){
	GEMDetId gemid(idpad.first);
	hasGEM_[gemid.station()-1] = true;
	gp_GEM[gemid.station()-1] = GeometryHelpers::globalPositionOfGEMPad(gem_g, idpad.second, gemid);
    }
}

void L1TDisplacedMuonPtAssignment::convertME0SegIntoGlobalpoints(){
    //FIXME: what if ME0 segment is not found
    hasME0_ = true;
    gp_ME0 = GeometryHelpers::globalPositionOfME0LCT(me0_g, segment_);
}



void L1TDisplacedMuonPtAssignment::calculatePositionPtBarrel(){}
void L1TDisplacedMuonPtAssignment::calculatePositionPtOverlap(){}
void L1TDisplacedMuonPtAssignment::calculatePositionPtEndcap()
{
  EvenOdd123 parity = EndcapTriggerPtAssignmentHelper::getParity(isEven[0], isEven[1],
                                                                 isEven[2], isEven[3]);

  // ignore invalid cases
  // put a warning message here!
  if (parity == EvenOdd123::Invalid)
    return;

  //ddY123 available and also need to consider whether GE21 is used or not!!!  FIXME
  ddY123 = EndcapTriggerPtAssignmentHelper::deltadeltaYcalculation(gp_st_layer3[0], gp_st_layer3[1], gp_st_layer3[2],
										  gp_st_layer3[1].eta(), parity);
  // lowest pT assigned
  positionPt_ = 2.0;

  // walk through the DDY LUT and assign the pT that matches
  const int etaSector = EndcapTriggerPtAssignmentHelper::GetEtaPartition_position(gp_ME[1].eta());
  for (int i=0; i<EndcapTriggerPtAssignmentHelper::NPtbins; i++){
    if (std::fabs(ddY123) <= EndcapTriggerPtAssignmentHelper::PositionPtDDYLUT[i][etaSector][int(parity)])
      positionPt_ = float(EndcapTriggerPtAssignmentHelper::PtBins[i]);
    else
      break;
  }
}

void L1TDisplacedMuonPtAssignment::calculateDirectionPtBarrel()
{
  // check case
  int dt_stub_case = 0;//getBarrelStubCase(has_stub_mb1, has_stub_mb2, has_stub_mb3, has_stub_mb4);

  float barrel_direction_pt;

  switch(dt_stub_case){
  case 0:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt2Stubs(dPhi_barrel_dir_12, "DT1_DT2");
    break;
  case 1:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt2Stubs(dPhi_barrel_dir_13, "DT1_DT3");
    break;
  case 2:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt2Stubs(dPhi_barrel_dir_14, "DT1_DT4");
    break;
  case 3:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt2Stubs(dPhi_barrel_dir_23, "DT2_DT3");
    break;
  case 4:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt2Stubs(dPhi_barrel_dir_24, "DT2_DT4");
    break;
  case 5:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt2Stubs(dPhi_barrel_dir_34, "DT3_DT4");
    break;
  case 6:
    // first dphi is x value, second dphi is y value!!!!
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt3or4Stubs(dPhi_barrel_dir_12, dPhi_barrel_dir_13, "DT1_DT2__DT1_DT3");
    break;
  case 7:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt3or4Stubs(dPhi_barrel_dir_12, dPhi_barrel_dir_14, "DT1_DT2__DT1_DT4");
    break;
  case 8:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt3or4Stubs(dPhi_barrel_dir_13, dPhi_barrel_dir_14, "DT1_DT3__DT1_DT4");
    break;
  case 9:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt3or4Stubs(dPhi_barrel_dir_23, dPhi_barrel_dir_24, "DT2_DT3__DT2_DT4");
    break;
  case 10:
    barrel_direction_pt = BarrelTriggerPtAssignmentHelper::getDirectionPt3or4Stubs(dPhi_barrel_dir_14, dPhi_barrel_dir_23, "DT1_DT4__DT2_DT3");
    break;
  default:
    // all else fails; assign lowest possible pT
    barrel_direction_pt = 2;
    break;
  };
  std::cout << barrel_direction_pt << std::endl;
}

void L1TDisplacedMuonPtAssignment::calculateDirectionPtOverlap(){}
//Medium: 1.6<eta<2.1, GE11 and GE21 are used 
//high: eta>2.1, GE11 and GE21, ME0 are usd
void L1TDisplacedMuonPtAssignment::calculateDirectionPtEndcapMedium(){
    EvenOdd123 parity = EndcapTriggerPtAssignmentHelper::getParity(isEven[0], isEven[1],//only first two station matters
                                                                 isEven[1], isEven[1]);

    if (not(hasGEM_[0] and hasGEM_[1])){
	return;
    }

    float xfactor = (gp_st_layer3[1].perp()/gp_st_layer3[0].perp()-1.0)/fabs(gp_st_layer3[0].z()-gp_st_layer3[1].z());
    float xfactor_st1 = xfactor*fabs(gp_GEM[0].z() - gp_st_layer3[0].z()); //use GE11
    phiM_st1 = EndcapTriggerPtAssignmentHelper::phiMomentum_Xfactor(gp_st_layer3[0].phi(), gp_GEM[0].phi(), xfactor_st1);
    float xfactor_st2 = xfactor*fabs(gp_GEM[1].z() - gp_st_layer3[1].z())/(xfactor*fabs(gp_st_layer3[0].z() - gp_st_layer3[1].z())+1);
    phiM_st2 = EndcapTriggerPtAssignmentHelper::phiMomentum_Xfactor(gp_st_layer3[1].phi(), gp_GEM[1].phi(), xfactor_st2);
    dPhi_dir_st1_st2  = EndcapTriggerPtAssignmentHelper::normalizePhi(phiM_st1 - phiM_st2);

    directionPt_ = 2.0;
    int etaSector = EndcapTriggerPtAssignmentHelper::GetEtaPartition_direction( gp_ME[1].eta() );
    for (int i=0; i<EndcapTriggerPtAssignmentHelper::NPtbins; i++){
	if (std::fabs(dPhi_dir_st1_st2) <= EndcapTriggerPtAssignmentHelper::DirectionbasedLUT[i][etaSector][int(parity)])
	    directionPt_ = float(EndcapTriggerPtAssignmentHelper::PtBins[i]);
	else
	    break;
    }

}

//high: eta>2.1, GE11 and GE21, ME0 are usd
void L1TDisplacedMuonPtAssignment::calculateDirectionPtEndcapHigh(){
    EvenOdd123 parity = EndcapTriggerPtAssignmentHelper::getParity(isEven[0], isEven[1],//only first two station matters
                                                                 isEven[1], isEven[1]);

    if (not(hasME0_ and hasGEM_[1])){
	return;
    }

    float xfactor = (gp_st_layer3[1].perp()/gp_st_layer3[0].perp()-1.0)/fabs(gp_st_layer3[0].z()-gp_st_layer3[1].z());
    float xfactor_st1 = xfactor*fabs(gp_ME0.z() - gp_st_layer3[0].z());
    phiM_st1 = EndcapTriggerPtAssignmentHelper::phiMomentum_Xfactor(gp_st_layer3[0].phi(), gp_ME0.phi(), xfactor_st1);//use ME0
    float xfactor_st2 = xfactor*fabs(gp_GEM[1].z() - gp_st_layer3[1].z())/(xfactor*fabs(gp_st_layer3[0].z() - gp_st_layer3[1].z())+1);
    phiM_st2 = EndcapTriggerPtAssignmentHelper::phiMomentum_Xfactor(gp_st_layer3[1].phi(), gp_GEM[1].phi(), xfactor_st2);
    dPhi_dir_st1_st2  = EndcapTriggerPtAssignmentHelper::normalizePhi(phiM_st1 - phiM_st2);

    directionPt_ = 2.0;
    int etaSector = EndcapTriggerPtAssignmentHelper::GetEtaPartition_direction( gp_ME[1].eta() );
    for (int i=0; i<EndcapTriggerPtAssignmentHelper::NPtbins; i++){
	if (std::fabs(dPhi_dir_st1_st2) <= EndcapTriggerPtAssignmentHelper::DirectionbasedLUT[i][etaSector][int(parity)])
	    directionPt_ = float(EndcapTriggerPtAssignmentHelper::PtBins[i]);
	else
	    break;
    }

}

void L1TDisplacedMuonPtAssignment::calculateHybridPtBarrel(){}
void L1TDisplacedMuonPtAssignment::calculateHybridPtOverlap(){}

void L1TDisplacedMuonPtAssignment::calculateHybridPtEndcapLow() {}
//Medium: eta<2.1, GE11 and GE21 are used 
void L1TDisplacedMuonPtAssignment::calculateHybridPtEndcapMedium(){
    int etaSector = EndcapTriggerPtAssignmentHelper::GetEtaPartition_hybrid( gp_ME[1].eta() );
    //ddY123, dPhi_dir_st1_st2 available, FIXME
    hybridPt_ = 2.0;
    if (fabs(ddY123)>=40 or fabs(dPhi_dir_st1_st2)>=1.0){//rejected by hybrid
	return;
    }
    int npar = int(EndcapTriggerPtAssignmentHelper::getParity(isEven[0], isEven[1],
                                                                 isEven[2], isEven[3]));
    //by defualt right now, GE21 is used, FIXME
    for (int i=0; i<EndcapTriggerPtAssignmentHelper::NPtbins; i++){
       if(EndcapTriggerPtAssignmentHelper::ellipse(EndcapTriggerPtAssignmentHelper::HybridLUT[i][etaSector][npar][0],
						   EndcapTriggerPtAssignmentHelper::HybridLUT[i][etaSector][npar][1],
						   EndcapTriggerPtAssignmentHelper::HybridLUT[i][etaSector][npar][2],
						   EndcapTriggerPtAssignmentHelper::HybridLUT[i][etaSector][npar][3],
						   EndcapTriggerPtAssignmentHelper::HybridLUT[i][etaSector][npar][4], 
						   ddY123, dPhi_dir_st1_st2) <=1)
	    hybridPt_ = EndcapTriggerPtAssignmentHelper::PtBins[i];
       /*else if(not(useGE21) and EndcapTriggerPtAssignmentHelper::ellipse(EndcapTriggerPtAssignmentHelper::HybridME21CSConly[i][etaSector][npar][0],
				                            EndcapTriggerPtAssignmentHelper::HybridME21CSConly[i][etaSector][npar][1],
				                            EndcapTriggerPtAssignmentHelper::HybridME21CSConly[i][etaSector][npar][2],
				                            EndcapTriggerPtAssignmentHelper::HybridME21CSConly[i][etaSector][npar][3],
				                            EndcapTriggerPtAssignmentHelper::HybridME21CSConly[i][etaSector][npar][4], 
							    ddY123, dPhi_dir_st1_st2) <=1)
	    hybrid_pt = EndcapTriggerPtAssignmentHelper::PtBins[i];
	    */
       else//make sure LUT is consitent
	    break;
    }
}
//high: eta>2.1, GE21, ME0 are usd, rectangular cut, rather than ellipse cut is used
void L1TDisplacedMuonPtAssignment::calculateHybridPtEndcapHigh(){
    int etaSector_p = EndcapTriggerPtAssignmentHelper::GetEtaPartition_position( gp_ME[1].eta() );
    int etaSector_d = EndcapTriggerPtAssignmentHelper::GetEtaPartition_direction( gp_ME[1].eta() );
    //ddY123, dPhi_dir_st1_st2 available, FIXME
    hybridPt_ = 2.0;
    if (fabs(ddY123)>=40 or fabs(dPhi_dir_st1_st2)>=1.0){//rejected by hybrid
	return;
    }
    int parity = int(EndcapTriggerPtAssignmentHelper::getParity(isEven[0], isEven[1],
                                                                 isEven[2], isEven[3]));
    for (int i=0; i<EndcapTriggerPtAssignmentHelper::NPtbins; i++){
	float ddY_cut = 2.0*EndcapTriggerPtAssignmentHelper::PositionPtDDYLUT[i][etaSector_p][int(parity)];
	float dphi_dir_cut =  2.0*EndcapTriggerPtAssignmentHelper::DirectionbasedLUT[i][etaSector_d][int(parity)];
	if (fabs(dPhi_dir_st1_st2) < dphi_dir_cut and fabs(ddY123) < ddY_cut)
	    hybridPt_ = EndcapTriggerPtAssignmentHelper::PtBins[i];
	else
	    break;
    }

}

int L1TDisplacedMuonPtAssignment::getBarrelStubCase(bool MB1, bool MB2, bool MB3, bool MB4)
{
  if (    MB1 and     MB2 and not MB3 and not MB4) return 0;
  if (    MB1 and not MB2 and     MB3 and not MB4) return 1;
  if (    MB1 and not MB2 and not MB3 and     MB4) return 2;
  if (not MB1 and     MB2 and     MB3 and not MB4) return 3;
  if (not MB1 and     MB2 and not MB3 and     MB4) return 4;
  if (not MB1 and not MB2 and     MB3 and     MB4) return 5;

  if (    MB1 and     MB2 and     MB3 and not MB4) return 6;
  if (    MB1 and     MB2 and not MB3 and     MB4) return 7;
  if (    MB1 and not MB2 and     MB3 and     MB4) return 8;
  if (not MB1 and     MB2 and     MB3 and     MB4) return 9;

  if (MB1 and MB2 and MB3 and MB4) return 10;

  return -1;
}
