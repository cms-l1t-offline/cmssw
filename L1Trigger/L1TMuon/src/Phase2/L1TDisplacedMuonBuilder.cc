#include "L1Trigger/L1TMuon/src/Phase2/L1TDisplacedMuonBuilder.h"
#include "L1Trigger/CSCCommonTrigger/interface/CSCConstants.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/normalizedPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "L1Trigger/L1TMuon/src/Phase2/GeometryHelpers.h"

using namespace L1TMuon;


L1TDisplacedMuonBuilder::L1TDisplacedMuonBuilder(const edm::ParameterSet& iConfig)
{
  fitter_.reset(new CSCComparatorDigiFitter());
  assignment_.reset(new L1TMuon::L1TDisplacedMuonPtAssignment(iConfig));
  recovery_.reset(new L1TMuon::L1TDisplacedMuonStubRecovery(iConfig));
}

L1TDisplacedMuonBuilder::~L1TDisplacedMuonBuilder()
{
  lcts_.clear();
  pads_.clear();
  dts_.clear();
}

void L1TDisplacedMuonBuilder::build(const CSCComparatorDigiCollection* comparators,
                                    const CSCCorrelatedLCTDigiCollection* lcts,
                                    const GEMPadDigiCollection* pads,
                                    const GEMCoPadDigiCollection* copads,
                                    const ME0SegmentCollection* segments,
                                    const l1t::EMTFTrackCollection* emtfTracks,
                                    const L1MuBMTrackCollection* bmtfTracks,
                                    const edm::Handle<l1t::RegionalMuonCandBxCollection>& bmtfMuonsH,
                                    const edm::Handle<l1t::RegionalMuonCandBxCollection>& omtfMuonsH,
                                    const edm::Handle<l1t::RegionalMuonCandBxCollection>& emtfMuonsH,
                                    std::unique_ptr<l1t::MuonPhase2BxCollection>& outMuons)
{
  const std::string functionName("L1TDisplacedMuonBuilder::build");

  const BXVector<l1t::RegionalMuonCand>& bmtfMuons(*bmtfMuonsH.product());
  const BXVector<l1t::RegionalMuonCand>& omtfMuons(*omtfMuonsH.product());
  const BXVector<l1t::RegionalMuonCand>& emtfMuons(*emtfMuonsH.product());

  for (int ibx = bmtfMuons.getFirstBX(); ibx <= bmtfMuons.getLastBX(); ++ibx) {
    for (unsigned int j = 0; j<bmtfMuons.size(ibx); ++j) {

      const auto& bmtfMuon = bmtfMuons.at(ibx,j);
      l1t::MuonPhase2 l1muon;

      // build the muon
      buildBMTFMuon(bmtfTracks, bmtfMuon, l1muon);

      // put muon in the collection
      outMuons->push_back( ibx, l1muon);
    }
  }

  for (int ibx = omtfMuons.getFirstBX(); ibx <= omtfMuons.getLastBX(); ++ibx) {
    for (unsigned int j = 0; j<omtfMuons.size(ibx); ++j) {

      const auto& omtfMuon = omtfMuons.at(ibx,j);
      l1t::MuonPhase2 l1muon;

      // build the muon
      buildOMTFMuon(omtfMuon, l1muon);

      // put muon in the collection
      outMuons->push_back( ibx, l1muon);
    }
  }

  for (int ibx = emtfMuons.getFirstBX(); ibx <= emtfMuons.getLastBX(); ++ibx) {
    for (unsigned int j = 0; j<emtfMuons.size(ibx); ++j) {

      const auto& emtfMuon = emtfMuons.at(ibx,j);
      l1t::MuonPhase2 l1muon;

      // build the muon
      buildEMTFMuon(comparators, lcts, pads, copads, emtfTracks, emtfMuon, l1muon);

      // put muon in the collection
      outMuons->push_back( ibx, l1muon);
    }
  }
}

void L1TDisplacedMuonBuilder::buildBMTFMuon(const L1MuBMTrackCollection* bmtfTacks,
                                           const l1t::RegionalMuonCand& bmtfMuon,
                                           l1t::MuonPhase2& muon)
{

}

void L1TDisplacedMuonBuilder::buildOMTFMuon(const l1t::RegionalMuonCand& omtfMuon,
                                           l1t::MuonPhase2& muon)
{

}

void L1TDisplacedMuonBuilder::buildEMTFMuon(const CSCComparatorDigiCollection* comps,
                                           const CSCCorrelatedLCTDigiCollection* lcts,
                                           const GEMPadDigiCollection* pads,
                                           const GEMCoPadDigiCollection* copads,
                                           const l1t::EMTFTrackCollection* emtfTracks,
                                           const l1t::RegionalMuonCand& emtfMuon,
                                           l1t::MuonPhase2& muon)
{
  // step 1: get the EMTF track associated to the regional track
  l1t::EMTFTrack bestTrack;
  float mindR = 999;
  for (const auto& track : *emtfTracks){
    const float emtf_eta = track.Eta();
    const float emtf_phi = normalizedPhi(track.Phi_glob());

    const float emtfMuon_eta = getRegionalMuonCandEta(emtfMuon);
    const float emtfMuon_phi = getRegionalMuonCandPhi(emtfMuon);
    float dr = reco::deltaR(emtf_eta, emtf_phi, emtfMuon_eta, emtfMuon_phi);
    if (dr<mindR){
      mindR = dr;
      bestTrack = track;
    }
  }

  // step 2: add the CSC/GEM stubs to the collection
  for (const auto& p : bestTrack.Hits()){
    if (p.Is_CSC()) {
      lcts_.emplace_back(p.CSC_DetId(),p.CSC_LCTDigi());
      const int station = p.CSC_DetId().station();
      hasCSC_[station-1] = true;
    }
    if (p.Is_GEM()) {
      pads_.emplace_back(p.GEM_DetId(),p.GEM_PadDigi());
      const int station = p.GEM_DetId().station();
      const int layer = p.GEM_DetId().layer();
      const int index(2*(station-1) + layer);
      hasGEM_[layer] = true;
    }
  }

  // step 3: match the ME0 stub to the muon
  recovery_->setME0Geometry(me0Geometry_);
  ME0Segment bestSegment;
  // recovery_->getBestMatchedME0(l1muon, segments, bestSegment);

  /*  Step 4: In case there are CSCCorrelatedLCTDigis,
   * first fit the comparator digis. This will be done with
   * the class CSCComparatorDigisFitter.
   */
  fitter_->setGeometry(cscGeometry_);
  for (const auto& stub: lcts_){
    std::vector<float> fit_phi_layers;
    std::vector<float> fit_z_layers;
    float fitRadius;
    const auto& detid(stub.first);

    // do not fit stubs in the outer rings -- no place in FPGA anyway!
    if (detid.ring()!=1 and detid.ring()!=4) continue;

    fitter_->fit(detid, stub.second, *comps,
                 fit_phi_layers, fit_z_layers, fitRadius);
    const int station = stub.first.station();
    if (station >= 1 and station <= 4) {
      cscFitPhiLayers_[station-1] = fit_phi_layers;
      cscFitZLayers_[station-1] = fit_z_layers;
      cscFitRLayers_[station-1] = fitRadius;
    }
  }

  // step 5
  /*
   * Then fit a straight line to them with the new class
   * DisplacedL1MuStubFitter. This is to better determine
   * the radius at each station.
   */
  // std::vector<float> xs;
  // std::vector<float> ys;
  // std::vector<float> zs;

  // const float z1 = 600;
  // const float z2 = 825;
  // const float z3 = 935;
  // const float z4 = 1020;

  // std::vector<float> allxs;
  // std::vector<float> allys;
  // std::vector<float> allzs;

  // xs.push_back(alpha_x + beta_x * z1*muon_sign);
  // xs.push_back(alpha_x + beta_x * z2*muon_sign);
  // xs.push_back(alpha_x + beta_x * z3*muon_sign);
  // xs.push_back(alpha_x + beta_x * z4*muon_sign);

  // ys.push_back(alpha_y + beta_y * z1*muon_sign);
  // ys.push_back(alpha_y + beta_y * z2*muon_sign);
  // ys.push_back(alpha_y + beta_y * z3*muon_sign);
  // ys.push_back(alpha_y + beta_y * z4*muon_sign);

  // int sign_z = int(event_.CSCTF_eta[j]/std::abs(event_.CSCTF_eta[j]));
  //   getPositionsStations(alpha_x, beta_x, alpha_y, beta_y,
  //                        allxs, allys, sign_z);

  // Step 5: Do stub recovery. Declare a new class L1TDisplacedMuonStubRecovery.
  if (!hasCSC_[0] or !hasCSC_[1] or !hasCSC_[2] or !hasCSC_[3]) {
    for (int i=0; i<4; ++i){
      if (!hasCSC_[i] and doStubRecovery_){
        CSCCorrelatedLCTDigiId recoveredLCT;
        recovery_->recoverCSCLCT(bestTrack, emtfTracks, lcts, i+1, recoveredLCT);
        lcts_.emplace_back(recoveredLCT);
      }
    }
  }

  // Step 5 bis: fit the comparator digis to the newly found stubs.
  for (const auto& stub: lcts_) {
    std::vector<float> fit_phi_layers;
    std::vector<float> fit_z_layers;
    float fitRadius;
    fitter_->fit(stub.first, stub.second, *comps,
                 fit_phi_layers, fit_z_layers, fitRadius);
    const int station = stub.first.station();
    if (station >= 1 and station <= 4) {
      cscFitPhiLayers_[station-1] = fit_phi_layers;
      cscFitZLayers_[station-1] = fit_z_layers;
      cscFitRLayers_[station-1] = fitRadius;
    }
  }
}




//       /*
//         Step 6: Fit CSCCorrelatedLCTDigis again with the new stubs.
//         [Alternative method: fetch all MPC LCT digis. Fit all possible combinations. Retain the combination with the lowest chi2/ndf]
//       */


//       /*
//         Step 7: Do GEM coincidence pad recovery. Declare a new class GEMPadRecovery (inherits from StubRecoveryBase) in the L1Mu trigger sector. If no GEM coincidence pads are found, do GEM pad recovery.
//       */

//       /*
//         Step 8: Calculate the direction of the stubs in each sector Barrel,
//         Overlap, EndcapLow, EndcapHigh. Declare a class StubDirectionCalculator.

//         DT stubs
//         CSC stubs separately
//         GEM-CSC stubs
//       */
//       assignment_->setCSCGeometry(cscGeometry_);
//       assignment_->setGEMGeometry(gemGeometry_);
//       assignment_->setDTGeometry(dtGeometry_);
//       assignment_->setME0Geometry(me0Geometry_);
//       assignment_->setTriggerPrimitives(pads_);
//       assignment_->setTriggerPrimitives(lcts_);
//       assignment_->setTriggerPrimitives(bestSegment);
//       assignment_->setMuon(l1muon);
//       assignment_->useGE21(false);
//       assignment_->useME0(false);
//       assignment_->setPtAssignmentMethod();

//       /*
// 	Step 9: Declare a pT assignment module DisplacedL1MuPTLUTBase. Feed directions into the pT assignment module. Declare derived classes DisplacedL1MuPTLUTBarrel, DisplacedL1MuPTLUTOverlap, DisplacedL1MuPTLUTEndcap.
//       */
//       /*
// 	Step 10: Declare classes PTAssignmentBase, PTAssignmentBarrel, PTAssignmentOverlap, PTAssignmentEndcap. Implement functions position based algorithm, direction based algorithm, hybrid based algorithm. Add parameter to choose the method. Add parameter to pick the efficiency threshold. Calculate the pT for each muon using the chosen method. Build a new L1Mu object with the new pT.
//       */
//       /*
// 	    Step 11: Produce the new collections with type L1Mu. Call this producer simPhaseDigisNoVtx, similar to the NoVtx algorithms at L2, L3 and Tracker.
//       */
//        outMuons->push_back( ibx, l1muon);
//       /*
// 	Step 12: Match objects to Tracker-tracks. Prompt L1Mu --> L1TkMu. Non-prompt L1Mu --> L1MuNoVtx.
//       */

//     }
//   }
// }
