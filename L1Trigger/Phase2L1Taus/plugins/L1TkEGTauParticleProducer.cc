// -*- C++ -*-
//
//
// Producer for a L1TkEGTauParticle
// 

// system include files
#include <memory>

// user include filesz
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1TrackTrigger/interface/L1TkEGTauParticle.h"
#include "L1Trigger/Phase2L1Taus/interface/L1TkEGTauEtComparator.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h" 

// inputs:
#include "DataFormats/L1TrackTrigger/interface/L1TrkTauParticle.h"

// for L1Tracks:
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"

#include <string>
#include "TMath.h"

//#define DEBUG

using namespace l1t ;
//
// class declaration
//

class L1TkEGTauParticleProducer : public edm::EDProducer {
public:
  
  typedef TTTrack< Ref_Phase2TrackerDigi_ >  L1TTTrackType;
  typedef std::vector< L1TTTrackType > L1TTTrackCollectionType;
  typedef edm::Ptr< L1TTTrackType > L1TTTrackRefPtr;
  typedef std::vector< L1TTTrackRefPtr > L1TTTrackRefPtr_Collection;
  typedef edm::Ref< edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_ > >, TTStub< Ref_Phase2TrackerDigi_ > > L1TTStubRef;
  typedef edm::Ref< EGammaBxCollection > EGammaRef ;
  typedef std::vector< EGammaRef > EGammaVectorRef ;

  explicit L1TkEGTauParticleProducer(const edm::ParameterSet&);
  ~L1TkEGTauParticleProducer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  class TrackPtComparator{
    
    unsigned int nFitParams_;
  public:
    TrackPtComparator(unsigned int nFitParams){ nFitParams_ = nFitParams;}
    bool operator() (const L1TTTrackRefPtr trackA, L1TTTrackRefPtr trackB ) const {
      return ( trackA->getMomentum(nFitParams_).perp() > trackB->getMomentum(nFitParams_).perp() );
    }
  };

  struct EGsEtComparator{ 
    bool operator() (const EGammaRef objA, const EGammaRef objB) const { return ( objA->et() > objB->et() ); }
  };

  float CorrectedEta(float eta, float zTrack);
  
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  math::XYZTLorentzVector GetCalibratedP4(const math::XYZTLorentzVector p4, 
					  const float ldgEG_Et); //iro

  void GetShrinkingConeSizes(float tk_pt,
			     const float shrinkCone_Constant,
			     const float sigCone_dRCutoff,
  			     float &sigCone_dRMin,
  			     float &sigCone_dRMax,
  			     float &isoCone_dRMin,
  			     float &isoCone_dRMax,
  			     const bool isoCone_useCone);
  
  // float CalculateRelIso(std::vector< L1TTTrackRefPtr > allTracks,
  // 			std::vector< unsigned int > clustTracksIndx,
  //                    const float deltaZ0_max, 
  // 			bool useIsoCone=false); 
  
  // ----------member data ---------------------------

  // Label of the objects which are created (e.g. "TkEG")
  std::string label;
 
  // L1 Tracker Taus
  float cfg_trkTau_minEt;               // Min eT applied on all L1TrkTaus [GeV]
  float cfg_trkTau_minEta;              // Min |eta| applied on all L1TrkTaus [unitless]
  float cfg_trkTau_maxEta;              // Max |eta| applied on all L1TrkTaus [unitless]
  unsigned int cfg_tk_nFitParams;       // Number of Fit Parameters: 4 or 5 ? (pT, eta, phi, z0, d0)

  // L1 EGammas 
  float cfg_eg_minEt;                   // Min eT applied on all L1EGammaCrystalClusters [GeV]
  float cfg_eg_minEta;                  // Min |eta| applied on all L1EGammaCrystalClusters [GeV]
  float cfg_eg_maxEta;                  // Max |eta| applied on all L1EGammaCrystalClusters [GeV]
  std::vector<unsigned int> cfg_eg_hwQualB; // Quality bit vector to use (|eta| <  1.52)
  std::vector<unsigned int> cfg_eg_hwQualE; // Quality bit vector to use (|eta| >= 1.52)

  // Shrinking Cone parameters
  float cfg_shrinkCone_Constant;        // Constant which is used for defining the opening of the signal cone : sigCone_dRMax = (cfg_shrinkCone_Constant)/(pT of the TkEG seed track) [GeV]
  float cfg_sigCone_cutoffDeltaR;       // Cutoff value for the maximum dR of the shrinking signal cone [unitless]
  bool  cfg_isoCone_useCone;            // Usage of isolation cone (true) or isolation annulus (false)
  float cfg_sigCone_dRMin;              // Min dR of signal cone [unitless]
  float cfg_isoCone_dRMax;              // Max dR of isolation cone/annulus [unitless]
  float sigCone_dRMax;                  // Max dR of signal cone [unitless]
  float isoCone_dRMin;                  // Min dR of isolation cone/annulus (= max dR of signal cone) [unitless]
  
  // EGs clustering parameters
  float cfg_maxInvMass_TkEGs;           // Max Invariant Mass of the Track+EG Cluster (including the L1TkEG seed L1TTTrack) [GeV/c^2]

  // Calibration
  bool  cfg_tkEGTau_calibrateEt;        // Apply EG-Et-dependent calibration factors to the tracks+EG candidates?

  const edm::EDGetTokenT< EGammaBxCollection > egToken;
  const edm::EDGetTokenT< EGammaBxCollection > egHGCalToken;
  const edm::EDGetTokenT< L1TrkTauParticleCollection > trktauToken;
} ;


//
// constructors and destructor
//
L1TkEGTauParticleProducer::L1TkEGTauParticleProducer(const edm::ParameterSet& iConfig) :
  egToken(consumes< EGammaBxCollection >(iConfig.getParameter<edm::InputTag>("L1EGammaInputTag"))),
  egHGCalToken(consumes< EGammaBxCollection >(iConfig.getParameter<edm::InputTag>("L1EGammaHGCalInputTag"))),
  trktauToken(consumes< L1TrkTauParticleCollection > (iConfig.getParameter<edm::InputTag>("L1TrkTauInputTag")))
  {
  
  label = iConfig.getParameter<std::string>("label");  // label of the collection produced
  
  // L1 Tracker Taus
  cfg_trkTau_minEt  = (float)iConfig.getParameter<double>("trkTau_minEt");
  cfg_trkTau_minEta = (float)iConfig.getParameter<double>("trkTau_minEta");
  cfg_trkTau_maxEta = (float)iConfig.getParameter<double>("trkTau_maxEta");
  cfg_tk_nFitParams = (unsigned int)iConfig.getParameter<unsigned int>("tk_nFitParams");

  // L1 EGammas
  cfg_eg_minEt   = (float)iConfig.getParameter<double>("eg_minEt");
  cfg_eg_minEta  = (float)iConfig.getParameter<double>("eg_minEta");
  cfg_eg_maxEta  = (float)iConfig.getParameter<double>("eg_maxEta");
  cfg_eg_hwQualB = iConfig.getParameter<std::vector<unsigned int>>("eg_hwQualB"); // iro
  cfg_eg_hwQualE = iConfig.getParameter<std::vector<unsigned int>>("eg_hwQualE"); // iro

  // Shrinking Cone parameters
  cfg_shrinkCone_Constant  = (float)iConfig.getParameter<double>("shrinkCone_Constant");
  cfg_sigCone_cutoffDeltaR = (float)iConfig.getParameter<double>("sigCone_cutoffDeltaR");
  cfg_isoCone_useCone      = (bool)iConfig.getParameter<bool>("isoCone_useCone");
  cfg_sigCone_dRMin        = (float)iConfig.getParameter<double>("sigCone_dRMin");
  cfg_isoCone_dRMax        = (float)iConfig.getParameter<double>("isoCone_dRMax");
  
  // EGs clustering parameters
  cfg_maxInvMass_TkEGs = (float)iConfig.getParameter<double>("maxInvMass_TkEGs");

  // Energy re-Calibration 
  cfg_tkEGTau_calibrateEt =  (bool)iConfig.getParameter<bool>("tkEGTau_calibrateEt");//iro
   
  produces<L1TkEGTauParticleCollection>(label);
}

L1TkEGTauParticleProducer::~L1TkEGTauParticleProducer() {
}

// ------------ method called to produce the data  ------------
void
L1TkEGTauParticleProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  using namespace edm;
  
  std::unique_ptr<L1TkEGTauParticleCollection> result(new L1TkEGTauParticleCollection);

  // Constants 
  const float pionMass  = 0.13957018;
    
  // the L1TrkTaus
  edm::Handle<L1TrkTauParticleCollection> trkTauHandle;
  iEvent.getByToken(trktauToken, trkTauHandle);
  L1TrkTauParticleCollection trkTauCollection = (*trkTauHandle.product());
  L1TrkTauParticleCollection::const_iterator trkTauIter;

  if ( !trkTauHandle.isValid() ) {
    LogError("L1TkEGTauParticleProducer")
      << "\nWarning: L1TrkTauParticleCollection not found in the event. Exit."
      << std::endl;
    return;
  } 

  // the L1EGamma objects
  // -- Barrel
  edm::Handle<EGammaBxCollection> eGammaHandle;
  iEvent.getByToken(egToken, eGammaHandle);  
  EGammaBxCollection eGammaCollection = (*eGammaHandle.product());

  // -- Endcap 
  edm::Handle<EGammaBxCollection> eGammaHGCalHandle;
  iEvent.getByToken(egHGCalToken, eGammaHGCalHandle);
  EGammaBxCollection eGammaHGCalCollection = (*eGammaHGCalHandle.product());

  EGammaBxCollection::const_iterator egIter;
  
  if( !eGammaHandle.isValid() ) {
    LogError("L1TkEGTauParticleProducer")
      << "\nWarning: L1EGammaParticleCollection not found in the event. Exit."
      << std::endl;
    return;
  }
  
  if( !eGammaHGCalHandle.isValid() ) {
    LogError("L1TkEGTauParticleProducer")
      << "\nWarning: L1EGammaHGCalParticleCollection not found in the event. Exit."
      << std::endl;
    return;
  }
  
  
  ///////////////////////////////////////////////////////////////
  //  Select Track-only Taus and EGs passing the quality criteria
  ///////////////////////////////////////////////////////////////
  
#ifdef DEBUG
  std::cout<<"\n--- Select all track-only taus passing the quality criteria"<<std::endl;
#endif

  std::vector< L1TrkTauParticleRef > SelTrkTausPtrs;
  unsigned int iTrkTau = 0;

  // For-loop: All the L1TrkTaus
  for (trkTauIter = trkTauHandle->begin(); trkTauIter != trkTauHandle->end(); ++trkTauIter) {
    
    /// Make a pointer to the L1TrkTaus
    edm::Ref< L1TrkTauParticleCollection > L1TrkTauParticleRef( trkTauHandle, iTrkTau++ );
    
    // Apply selection criteria on the L1TrkTaus
    float Et = trkTauIter -> et();
    float Eta = trkTauIter -> eta();
    //float Eta = trkTauIter -> getSeedTrk() -> getMomentum(cfg_tk_nFitParams).eta();

    if ( Et < cfg_trkTau_minEt ) continue;
    if ( fabs(Eta) < cfg_trkTau_minEta ) continue;
    if ( fabs(Eta) > cfg_trkTau_maxEta ) continue;
    
    SelTrkTausPtrs.push_back(L1TrkTauParticleRef);

  }// End-loop: All the L1TrkTaus

  // ---------------------------------

#ifdef DEBUG
  std::cout<<"\n--- Select all EGs passing the quality criteria"<<std::endl;
#endif

  std::vector< EGammaRef > SelEGsPtrs;
  unsigned int ieg = 0;

  // For-loop: All the L1EGs (Barrel)
  for (egIter = eGammaCollection.begin(); egIter != eGammaCollection.end();  ++egIter) {
    edm::Ref< EGammaBxCollection > EGammaRef( eGammaHandle, ieg++ );
    
    float Et   = egIter -> et();
    float Eta  = egIter -> eta();
    int hwQual = egIter -> hwQual();
    
    if (Et < cfg_eg_minEt) continue;
    if (fabs(Eta) < cfg_eg_minEta) continue;
    if (fabs(Eta) > cfg_eg_maxEta) continue;
    // if (hwQual != cfg_eg_hwQualB) continue; // iro
    if (std::find(cfg_eg_hwQualB.begin(), cfg_eg_hwQualB.end(), hwQual) == cfg_eg_hwQualB.end()) continue; // iro
#ifdef DEBUG
    std::cout << "Barrel) hwQual = " << hwQual << std::endl;
 #endif

    SelEGsPtrs.push_back(EGammaRef);

  }// End-loop: All the L1EGs (Barrel)

  // Re-initialize EGs counter to loop over HGCal EGs
  ieg = 0;

  // For-loop: All the L1EGs (Endcap)
  for (egIter = eGammaHGCalCollection.begin(); egIter != eGammaHGCalCollection.end();  ++egIter) {
    edm::Ref< EGammaBxCollection > EGammaRef( eGammaHGCalHandle, ieg++ );
    
    float Et   = egIter -> et();
    float Eta  = egIter -> eta();
    int hwQual = egIter -> hwQual();

    if (Et < cfg_eg_minEt) continue;
    if (fabs(Eta) < cfg_eg_minEta) continue;
    if (fabs(Eta) > cfg_eg_maxEta) continue;
    if (std::find(cfg_eg_hwQualE.begin(), cfg_eg_hwQualE.end(), hwQual) == cfg_eg_hwQualE.end()) continue; // iro

#ifdef DEBUG
    std::cout << "Endcap) hwQual = " << hwQual << std::endl; //iro
#endif
    SelEGsPtrs.push_back(EGammaRef);

  }// End-loop: All the L1EGs (Endcap)
  
  // Sort by ET all selected L1EGs
  std::sort( SelEGsPtrs.begin(), SelEGsPtrs.end(), EGsEtComparator() ); 


  ///////////////////////////////////////////////////////////////
  //  Tracks + EG Algorithm
  ///////////////////////////////////////////////////////////////

#ifdef DEBUG
  std::cout << "\n\t=== Tracks + EG Algorithm" <<std::endl;
#endif

  /*
  Description: Use as input the L1TrkTau particles (track-only taus) produced
               by the L1TrkTauParticleProducer with the applied selections
               and isolation given from the L1TrkTauParticleProducer_cfi.py
               configuration file.
               Starting from all the selected L1TrkTau particles (which are
               satisfying the eta restriction) we use the seed track to add
               also EG clusters to the tau candidates.
  */

  // For-loop: All the selected L1TrkTaus
  for ( unsigned int i=0; i < SelTrkTausPtrs.size(); i++ ){

    L1TrkTauParticleRef iTrkTau = SelTrkTausPtrs.at(i);
    
    // Retrieve seed track and its properties
    L1TTTrackRefPtr iSeedTrk = iTrkTau->getSeedTrk();
    float iPt  = iSeedTrk->getMomentum(cfg_tk_nFitParams).perp();
    float iEta = iSeedTrk->getMomentum(cfg_tk_nFitParams).eta();
    float iPhi = iSeedTrk->getMomentum(cfg_tk_nFitParams).phi();
    float iz0  = iSeedTrk->getPOCA(cfg_tk_nFitParams).z();

    // Get the sizes of the shrinking cone to be used for the EGs clustering
    GetShrinkingConeSizes(iPt, cfg_shrinkCone_Constant, cfg_sigCone_cutoffDeltaR, cfg_sigCone_dRMin, sigCone_dRMax, isoCone_dRMin, cfg_isoCone_dRMax, cfg_isoCone_useCone);

#ifdef DEBUG
    std::cout<<"Shrinking cone for tau-seed with pT = "<< iPt <<" GeV: sigCone_dRMin = "<< cfg_sigCone_dRMin <<"  , sigCone_dRMax = "<< sigCone_dRMax <<"  , isoCone_dRMi\
n = "<< isoCone_dRMin <<"  , isoCone_dRMax = "<< cfg_isoCone_dRMax <<std::endl;
#endif

    // Retrieve the properties of the track tau candidate (track cluster)
    std::vector<L1TTTrackRefPtr> TrackCluster = iTrkTau->getTrks();
    math::XYZTLorentzVector p4_trks = iTrkTau->p4();
    float iso = iTrkTau->getIso();

    // EGs clustering 
    std::vector<EGammaRef> EGcluster;
    std::vector<unsigned int> EGclusterIndx;
    EGcluster.clear();
    EGclusterIndx.clear();

    math::XYZTLorentzVector p4_egs, p4_tmp, p4_cand_tmp;
    p4_egs.SetCoordinates(0.,0.,0.,0.);
    
    // Cluster EGs until you reach the mass cutoff for the TkEGTau Candidate
    for ( unsigned int j=0; j < SelEGsPtrs.size(); j++ ){
      
      EGammaRef jEG = SelEGsPtrs.at(j);
      float jEt  = jEG->et();
      float jEta = jEG->eta();
      float jPhi = jEG->phi();
      
      // Apply dR criteria for EG clustering
      float deltaR = reco::deltaR(iEta, iPhi, CorrectedEta(jEta, iz0), jPhi);
      if (deltaR > sigCone_dRMax) continue;
      
      // Calculate p4 of EG
      float jPt      = sqrt( (jEt/(sin(2*atan(exp(-jEta)))))*(jEt/(sin(2*atan(exp(-jEta))))) - pionMass*pionMass )*sin(2*atan(exp(-jEta)));
      Double_t px    = jPt*cos(jPhi);
      Double_t py    = jPt*sin(jPhi);
      Double_t theta = 2*atan(exp(-jEta));
      Double_t pz    = jPt/tan(theta);
      Double_t p     = sqrt(jPt*jPt+pz*pz);
      Double_t e     = sqrt(pionMass*pionMass+p*p);
      
      p4_tmp.SetCoordinates(px,py,pz,e);
      
      // Calculate the p4 of the candidate (current p4 + last EG)
      p4_cand_tmp = p4_trks + p4_egs + p4_tmp;
      
      // Apply mass cut off to the candidate 
      if (p4_cand_tmp.M() > cfg_maxInvMass_TkEGs) continue;

      // If the TkEG candidates mass remains below the cutoff value add the EG to the cluster and add its p4
      p4_egs += p4_tmp;
      
      EGcluster.push_back(jEG);
#ifdef DEBUG
      std::cout << "i = " << i << ", j = " << j << ", jEG->et() = " << jEG->et() << std::endl;
#endif
      EGclusterIndx.push_back(j);
      
    }// EGs Clustering
    
     
    // Build the tau candidate
    const int tkEG_nEGS = EGcluster.size();
    math::XYZTLorentzVector p4_total;
    math::XYZTLorentzVector p4_cal;
    math::XYZTLorentzVector p4_final;
    p4_total = p4_trks + p4_egs;

    // Only calibrate the tau candidate if an EG is associated with it
    if (tkEG_nEGS > 0) p4_cal = GetCalibratedP4(p4_total, EGcluster.at(0)->et() ); //iro
    else p4_cal = p4_total;
  
    // Apply EG-Et-dependent calibration factors to the tracks+EG candidates? 
    if (cfg_tkEGTau_calibrateEt) p4_final = p4_cal; // iro
    else p4_final = p4_total;

    // Create the tracks+EG tau candidate
    L1TkEGTauParticle trkEG(p4_final, TrackCluster, EGcluster, iso);

    // Keep the tracks+EGs tau candidate 
    result -> push_back( trkEG );
    
  }// End-loop: All the L1TrkTaus
#ifdef DEBUG
  std::cout << "\n" << std::endl;
#endif

  // Sort the TkEG candidates by eT before saving to the event 
  sort( result->begin(), result->end(), L1TkEGTau::EtComparator() );
  
  iEvent.put(std::move(result), label );
  
}


// --------------------------------------------------------------------------------------
math::XYZTLorentzVector L1TkEGTauParticleProducer::GetCalibratedP4(const math::XYZTLorentzVector p4,
								   const float ldgEG_Et) //iro
{
  math::XYZTLorentzVector p4_cal;
  
  const float px   = p4.px();
  const float py   = p4.py();
  const float pz   = p4.pz();
  const float e    = p4.e();
  float e_corr     = 0.0;
  float corrFactor = 1.0;

  // Correction factors
  const float corrFactor_0to5GeV  = 1.0 + 0.04; // MTD5 samples
  const float corrFactor_5to50GeV = 1.0 - 0.07; // MTD5 samples
  const float corrFactor_GE50GeV  = 1.0 - 0.07; // MTD5 samples

  // Assign the correct correction factor
  if (ldgEG_Et < 5)
    {
      corrFactor = corrFactor_0to5GeV;
    }
  else if ( (ldgEG_Et >= 5) && (ldgEG_Et < 50) )
    {
      corrFactor = corrFactor_5to50GeV;
    }
  else
    {
      corrFactor = corrFactor_GE50GeV;
    }
  
  // Fill the lorentz vector with calibrated value of energy
  e_corr = e*corrFactor;
  p4_cal.SetCoordinates(px, py, pz, e_corr);

#ifdef DEBUG
  std::cout << "ldgEG_Et = " << ldgEG_Et << ", Et  = " << p4.Et()     << ", Eta  = " << p4.Eta()       << ", Phi  = " << p4.Phi()      << std::endl;
  std::cout << "ldgEG_Et = " << ldgEG_Et << ", Et_ = " << p4_cal.Et() << ", Eta_ = " << p4_cal.Eta()   << ", Phi_ = " << p4_cal.Phi()  << std::endl;
  std::cout << "\n" << std::endl;
#endif
  

  return p4_cal;
  
}
void L1TkEGTauParticleProducer::GetShrinkingConeSizes(float tk_pt,
						   const float shrinkCone_Constant,
						   const float sigCone_dRCutoff,
						   float &sigCone_dRMin,
						   float &sigCone_dRMax,
						   float &isoCone_dRMin,
						   float &isoCone_dRMax,
						   const bool isoCone_useCone)
{
  // Signal cone
  double signalCone_min = sigCone_dRMin;
  double signalCone_max = (shrinkCone_Constant)/(tk_pt);
  if (signalCone_max > sigCone_dRCutoff) signalCone_max = sigCone_dRCutoff;
 
  // Isolation cone/annulus
  double isoCone_min;
  if (isoCone_useCone) isoCone_min = 0.0;
  else isoCone_min = signalCone_max;
  double isoCone_max = isoCone_dRMax;
      
  // Assign signal and isolation cone sizes
  sigCone_dRMin = signalCone_min;
  sigCone_dRMax = signalCone_max;  
  isoCone_dRMin = isoCone_min;
  isoCone_dRMax = isoCone_max;

  return;
}

// --------------------------------------------------------------------------------------

float L1TkEGTauParticleProducer::CorrectedEta(float eta, float zTrack)  {

  // Correct the eta of the L1EG object once we know the zTrack
  // (normaly we use the zvertex but since we care about the dR of the EG and the track it is not needed)
 
  
  bool IsBarrel = ( fabs(eta) < 1.479 );
  float REcal = 129. ;
  float ZEcal = 315.4 ;
  
  float theta = 2. * TMath::ATan( TMath::Exp( - eta ) );
  if (theta < 0) theta = theta + TMath::Pi();
  float tantheta = TMath::Tan( theta );
  
  float delta;
  if (IsBarrel) {
    delta = REcal / tantheta ;
  }
  else {
    if (theta > 0) delta =  ZEcal;
    if (theta < 0) delta = -ZEcal;
  }
  
  float tanthetaprime = delta * tantheta / (delta - zTrack );
  
  float thetaprime = TMath::ATan( tanthetaprime );
  if (thetaprime < 0) thetaprime = thetaprime + TMath::Pi();
  
  float etaprime = -TMath::Log( TMath::Tan( thetaprime / 2.) );
  return etaprime;
  
}

// ------------ method called once each job just before starting event loop  ------------
void
L1TkEGTauParticleProducer::beginJob()
{
  // std::cout << "L1TkEGTauParticleProducer::beginJob() " << std::endl;
}

// ------------ method called once each job just after ending the event loop  ------------
void
L1TkEGTauParticleProducer::endJob() {
  // std::cout << "L1TkEGTauParticleProducer::endJob() " << std::endl;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TkEGTauParticleProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TkEGTauParticleProducer);



