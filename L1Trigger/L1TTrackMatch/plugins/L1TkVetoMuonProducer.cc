// input: L1TkTracks and  RegionalMuonCand (standalone with component details)
// veto the prompt regionalmuoncand

// user include files
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkMuonParticle.h"
#include "DataFormats/L1TrackTrigger/interface/L1TkMuonParticleFwd.h"
#include "L1Trigger/L1TMuon/interface/MicroGMTConfiguration.h"
#include "L1Trigger/L1TMuonEndCap/interface/Common.h"

// system include files
#include <memory>
#include <string>

using namespace l1t;

class L1TkVetoMuonProducer : public edm::EDProducer {
public:

  typedef TTTrack< Ref_Phase2TrackerDigi_ >  L1TTTrackType;
  typedef std::vector< L1TTTrackType > L1TTTrackCollectionType;

  struct PropState { //something simple, imagine it's hardware emulation
    PropState() :
      pt(-99),  eta(-99), phi(-99),
      sigmaPt(-99),  sigmaEta(-99), sigmaPhi(-99),
      valid(false) {}
    float pt;
    float eta;
    float phi;
    float sigmaPt;
    float sigmaEta;
    float sigmaPhi;
    bool valid;

  };

  explicit L1TkVetoMuonProducer(const edm::ParameterSet&);
  ~L1TkVetoMuonProducer() {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:

  virtual void produce(edm::Event&, const edm::EventSetup&);
  PropState propagateToGMT(const L1TTTrackType& l1tk) const;

  // the TP algorithm
  void runOnMTFCollection(const edm::Handle<RegionalMuonCandBxCollection>&,
                          const edm::Handle<L1TTTrackCollectionType>&,
                          RegionalMuonCandBxCollection& mtf_loose,
                          RegionalMuonCandBxCollection& mtf_medium,
                          RegionalMuonCandBxCollection& mtf_tight) const;

  float ETAMIN_;
  float ETAMAX_;
  float ZMAX_;             // |z_track| < ZMAX in cm
  float CHI2MAX_;
  float PTMINTRA_;
  float DRmax_;
  int nStubsmin_ ;         // minimum number of stubs
  bool correctGMTPropForTkZ_;
  bool use5ParameterFit_;

  float ptLoose_;
  float ptMedium_;
  float ptTight_;

  const edm::EDGetTokenT< RegionalMuonCandBxCollection > bmtfToken;
  const edm::EDGetTokenT< RegionalMuonCandBxCollection > omtfToken;
  const edm::EDGetTokenT< RegionalMuonCandBxCollection > emtfToken;
  const edm::EDGetTokenT< L1TTTrackCollectionType > trackToken;
};


L1TkVetoMuonProducer::L1TkVetoMuonProducer(const edm::ParameterSet& iConfig) :
  bmtfToken  (consumes< RegionalMuonCandBxCollection >(iConfig.getParameter<edm::InputTag>("L1BMTFInputTag"))),
  omtfToken  (consumes< RegionalMuonCandBxCollection >(iConfig.getParameter<edm::InputTag>("L1OMTFInputTag"))),
  emtfToken  (consumes< RegionalMuonCandBxCollection >(iConfig.getParameter<edm::InputTag>("L1EMTFInputTag"))),
  trackToken (consumes< L1TTTrackCollectionType > (iConfig.getParameter<edm::InputTag>("L1TrackInputTag")))
{
   ETAMIN_ = (float)iConfig.getParameter<double>("ETAMIN");
   ETAMAX_ = (float)iConfig.getParameter<double>("ETAMAX");
   ZMAX_ = (float)iConfig.getParameter<double>("ZMAX");
   CHI2MAX_ = (float)iConfig.getParameter<double>("CHI2MAX");
   PTMINTRA_ = (float)iConfig.getParameter<double>("PTMINTRA");
   DRmax_ = (float)iConfig.getParameter<double>("DRmax");
   nStubsmin_ = iConfig.getParameter<int>("nStubsmin");
   correctGMTPropForTkZ_ = iConfig.getParameter<bool>("correctGMTPropForTkZ");
   use5ParameterFit_     = iConfig.getParameter<bool>("use5ParameterFit");

   ptLoose_ = (float)iConfig.getParameter<double>("ptLoose");
   ptMedium_ = (float)iConfig.getParameter<double>("ptMedium");
   ptTight_ = (float)iConfig.getParameter<double>("ptTight");

   produces<RegionalMuonCandBxCollection>("BMTFLoose");
   produces<RegionalMuonCandBxCollection>("EMTFLoose");
   produces<RegionalMuonCandBxCollection>("OMTFLoose");

   produces<RegionalMuonCandBxCollection>("BMTFMedium");
   produces<RegionalMuonCandBxCollection>("EMTFMedium");
   produces<RegionalMuonCandBxCollection>("OMTFMedium");

   produces<RegionalMuonCandBxCollection>("BMTFTight");
   produces<RegionalMuonCandBxCollection>("EMTFTight");
   produces<RegionalMuonCandBxCollection>("OMTFTight");
}

// ------------ method called to produce the data  ------------
void
L1TkVetoMuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // the L1Mu objects
  edm::Handle<RegionalMuonCandBxCollection> l1bmtfH;
  edm::Handle<RegionalMuonCandBxCollection> l1omtfH;
  edm::Handle<RegionalMuonCandBxCollection> l1emtfH;

  iEvent.getByToken(bmtfToken, l1bmtfH);
  iEvent.getByToken(omtfToken, l1omtfH);
  iEvent.getByToken(emtfToken, l1emtfH);

  // the L1Tracks
  edm::Handle<L1TTTrackCollectionType> l1tksH;
  iEvent.getByToken(trackToken, l1tksH);

  std::unique_ptr<RegionalMuonCandBxCollection> oc_l1bmtf_loose(new RegionalMuonCandBxCollection());
  std::unique_ptr<RegionalMuonCandBxCollection> oc_l1bmtf_medium(new RegionalMuonCandBxCollection());
  std::unique_ptr<RegionalMuonCandBxCollection> oc_l1bmtf_tight(new RegionalMuonCandBxCollection());

  std::unique_ptr<RegionalMuonCandBxCollection> oc_l1omtf_loose(new RegionalMuonCandBxCollection());
  std::unique_ptr<RegionalMuonCandBxCollection> oc_l1omtf_medium(new RegionalMuonCandBxCollection());
  std::unique_ptr<RegionalMuonCandBxCollection> oc_l1omtf_tight(new RegionalMuonCandBxCollection());

  std::unique_ptr<RegionalMuonCandBxCollection> oc_l1emtf_loose(new RegionalMuonCandBxCollection());
  std::unique_ptr<RegionalMuonCandBxCollection> oc_l1emtf_medium(new RegionalMuonCandBxCollection());
  std::unique_ptr<RegionalMuonCandBxCollection> oc_l1emtf_tight(new RegionalMuonCandBxCollection());

  // process each of the MTF collections separately!
  runOnMTFCollection(l1bmtfH, l1tksH, *oc_l1bmtf_loose, *oc_l1bmtf_medium, *oc_l1bmtf_tight);
  runOnMTFCollection(l1omtfH, l1tksH, *oc_l1omtf_loose, *oc_l1omtf_medium, *oc_l1omtf_tight);
  runOnMTFCollection(l1emtfH, l1tksH, *oc_l1emtf_loose, *oc_l1emtf_medium, *oc_l1emtf_tight);

  // put the new track+muon objects in the event!
  iEvent.put( std::move(oc_l1bmtf_loose));
  iEvent.put( std::move(oc_l1bmtf_medium));
  iEvent.put( std::move(oc_l1bmtf_tight));

  iEvent.put( std::move(oc_l1omtf_loose));
  iEvent.put( std::move(oc_l1omtf_medium));
  iEvent.put( std::move(oc_l1omtf_tight));

  iEvent.put( std::move(oc_l1emtf_loose));
  iEvent.put( std::move(oc_l1emtf_medium));
  iEvent.put( std::move(oc_l1emtf_tight));
};

void
L1TkVetoMuonProducer::runOnMTFCollection(const edm::Handle<RegionalMuonCandBxCollection>& muonH,
                                         const edm::Handle<L1TTTrackCollectionType>& l1tksH,
                                         RegionalMuonCandBxCollection& mtf_loose,
                                         RegionalMuonCandBxCollection& mtf_medium,
                                         RegionalMuonCandBxCollection& mtf_tight) const
{
  // initialize the collections
  mtf_loose.setBXRange(0,0);
  mtf_medium.setBXRange(0,0);
  mtf_tight.setBXRange(0,0);

  const L1TTTrackCollectionType& l1tks = (*l1tksH.product());
  const RegionalMuonCandBxCollection& l1mtfs = (*muonH.product());

  int imu = 0;
  for (auto l1mu = l1mtfs.begin(0); l1mu != l1mtfs.end(0);  ++l1mu){ // considering BX = only

    edm::Ref< RegionalMuonCandBxCollection > l1muRef( muonH, imu );
    imu++;

    float l1mu_eta = l1mu->hwEta()*0.010875;
    // get the global phi
    float l1mu_phi = MicroGMTConfiguration::calcGlobalPhi( l1mu->hwPhi(), l1mu->trackFinderType(), l1mu->processor() )*2*M_PI/576.;

    float l1mu_feta = fabs( l1mu_eta );
    if (l1mu_feta < ETAMIN_) continue;
    if (l1mu_feta > ETAMAX_) continue;

    PropState matchProp;

    bool isLooseVeto;
    bool isMediumVeto;
    bool isTightVeto;

    for (const auto& l1tk : l1tks ){
      unsigned int nPars = 4;
      if (use5ParameterFit_) nPars = 5;

      float l1tk_pt = l1tk.getMomentum(nPars).perp();
      if (l1tk_pt < PTMINTRA_) continue;

      float l1tk_z  = l1tk.getPOCA(nPars).z();
      if (fabs(l1tk_z) > ZMAX_) continue;

      float l1tk_chi2 = l1tk.getChi2(nPars);
      if (l1tk_chi2 > CHI2MAX_) continue;

      int l1tk_nstubs = l1tk.getStubRefs().size();
      if ( l1tk_nstubs < nStubsmin_) continue;

      const PropState& pstate = propagateToGMT(l1tk);
      if (!pstate.valid) continue;

      const double dR_l1Mu_l1Tk_prop = reco::deltaR(matchProp.eta, matchProp.phi,
                                                    l1mu_eta, l1mu_phi);

      if (dR_l1Mu_l1Tk_prop < DRmax_){

        const double l1Tk_pt = l1tk.getMomentum().perp();

        // if the muon is tight vetoed, you can break from the track loop
        if (l1Tk_pt >= ptTight_) {
          isTightVeto  = true;
          isMediumVeto = true;
          isLooseVeto  = true;
          break;
        }

        // for medium and loose veto, still need to keep looping
        // until all tracks have been checked
        if (l1Tk_pt >= ptMedium_) {
          isMediumVeto = true;
          isLooseVeto  = true;
        }

        if (l1Tk_pt >= ptLoose_) {
          isLooseVeto  = true;
        }
      }
    }// over l1tks

    // insert the muons if they were not vetoed
    if (not isLooseVeto)  {
      mtf_loose.push_back(0, *l1mu);
    }
    // not medium also means not loose!
    if (not isMediumVeto) {
      mtf_loose.push_back(0, *l1mu);
      mtf_medium.push_back(0, *l1mu);
    }
    // not tight also means not medium and not not loose!
    if (not isTightVeto) {
      mtf_loose.push_back(0, *l1mu);
      mtf_medium.push_back(0, *l1mu);
      mtf_tight.push_back(0, *l1mu);
    }
  }//over l1mus
}




// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TkVetoMuonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


L1TkVetoMuonProducer::PropState L1TkVetoMuonProducer::propagateToGMT(const L1TkVetoMuonProducer::L1TTTrackType& tk) const {
  auto p3 = tk.getMomentum();
  float tk_pt = p3.perp();
  float tk_p = p3.mag();
  float tk_eta = p3.eta();
  float tk_aeta = std::abs(tk_eta);
  float tk_phi = p3.phi();
  float tk_q = tk.getRInv()>0? 1.: -1.;
  float tk_z  = tk.getPOCA().z();
  if (!correctGMTPropForTkZ_) tk_z = 0;

  L1TkVetoMuonProducer::PropState dest;
  if (tk_p<3.5 ) return dest;
  if (tk_aeta <1.1 && tk_pt < 3.5) return dest;
  if (tk_aeta > 2.5) return dest;

  //0th order:
  dest.valid = true;

  float dzCorrPhi = 1.;
  float deta = 0;
  float etaProp = tk_aeta;

  if (tk_aeta < 1.1){
    etaProp = 1.1;
    deta = tk_z/550./cosh(tk_aeta);
  } else {
    float delta = tk_z/850.; //roughly scales as distance to 2nd station
    if (tk_eta > 0) delta *=-1;
    dzCorrPhi = 1. + delta;

    float zOzs = tk_z/850.;
    if (tk_eta > 0) deta = zOzs/(1. - zOzs);
    else deta = zOzs/(1.+zOzs);
    deta = deta*tanh(tk_eta);
  }
  float resPhi = tk_phi - 1.464*tk_q*cosh(1.7)/cosh(etaProp)/tk_pt*dzCorrPhi - M_PI/144.;
  if (resPhi > M_PI) resPhi -= 2.*M_PI;
  if (resPhi < -M_PI) resPhi += 2.*M_PI;

  dest.eta = tk_eta + deta;
  dest.phi = resPhi;
  dest.pt = tk_pt; //not corrected for eloss

  dest.sigmaEta = 0.100/tk_pt; //multiple scattering term
  dest.sigmaPhi = 0.106/tk_pt; //need a better estimate for these
  return dest;
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TkVetoMuonProducer);
