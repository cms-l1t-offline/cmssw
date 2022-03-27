// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "DataFormats/Math/interface/LorentzVector.h"

// L1 objects
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1TCorrelator/interface/TkJet.h"
#include "DataFormats/L1TCorrelator/interface/TkJetFwd.h"
#include "DataFormats/L1Trigger/interface/Vertex.h"

// geometry
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "RecoJets/JetProducers/plugins/VirtualJetProducer.h"

//mc
#include "SimTracker/TrackTriggerAssociation/interface/TTTrackAssociationMap.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

#include "DataFormats/L1TCorrelator/interface/TPJet.h"
#include "DataFormats/L1TCorrelator/interface/TPJetFwd.h"

#include <string>
#include "TMath.h"
#include "TH1.h"

using namespace l1t;
using namespace edm;
using namespace std;

//////////////////////////////
//                          //
//     CLASS DEFINITION     //
//                          //
//////////////////////////////

class L1TrackingParticleJetProducer : public edm::stream::EDProducer<> {

public:

  explicit L1TrackingParticleJetProducer(const edm::ParameterSet&);
  ~L1TrackingParticleJetProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob();
  void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob();

  bool DebugMode;
  double coneSize_;        // Use anti-kt with this cone size
  bool displaced_;
  // track selection criteria
  // in [cm]
  float TP_maxZ0;
  float TP_maxEta;
  float TP_minPt;
  int TP_minNStub;
  int TP_minNStubLayer;

  edm::InputTag TrackingParticleInputTag;
  edm::EDGetTokenT<std::vector<TrackingParticle>> TrackingParticleToken_;

  edm::InputTag MCTruthStubInputTag;
  edm::EDGetTokenT<TTStubAssociationMap<Ref_Phase2TrackerDigi_>> ttStubMCTruthToken_;
  
};

L1TrackingParticleJetProducer::L1TrackingParticleJetProducer(edm::ParameterSet const& iConfig) : 
  TrackingParticleToken_(consumes<std::vector<TrackingParticle>>(iConfig.getParameter<edm::InputTag>("TrackingParticleInputTag"))){

  DebugMode = iConfig.getParameter<bool>("DebugMode");
  coneSize_ = (float)iConfig.getParameter<double>("coneSize");
  displaced_ = iConfig.getParameter<bool>("displaced");

  // TrackingParticleInputTag = iConfig.getParameter<edm::InputTag>("TrackingParticleInputTag");
  // TrackingParticleToken_ = consumes<std::vector<TrackingParticle>>(TrackingParticleInputTag);

  MCTruthStubInputTag = iConfig.getParameter<edm::InputTag>("MCTruthStubInputTag");
  ttStubMCTruthToken_ = consumes<TTStubAssociationMap<Ref_Phase2TrackerDigi_>>(MCTruthStubInputTag);

  TP_minNStub = iConfig.getParameter<int>("TP_minNStub");
  TP_minNStubLayer = iConfig.getParameter<int>("TP_minNStubLayer");
  TP_minPt = iConfig.getParameter<double>("TP_minPt");
  TP_maxEta = iConfig.getParameter<double>("TP_maxEta");
  TP_maxZ0 = iConfig.getParameter<double>("TP_maxZ0");

  if (displaced_)
    produces<TPJetCollection>("L1TrackingParticleJetsExtended");
  else
    produces<TPJetCollection>("L1TrackingParticleJets");

  // produces<TPJetCollection>("L1TrackingParticleJets");

  }

//  destructor
L1TrackingParticleJetProducer::~L1TrackingParticleJetProducer() {}

  // producer
void L1TrackingParticleJetProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // std::unique_ptr<TkJetCollection> L1TrackingParticleJets(new TkJetCollection);
  std::unique_ptr<TPJetCollection> L1TrackingParticleJets(new TPJetCollection);
  
  edm::Handle<std::vector<TrackingParticle>> TrackingParticleHandle;
  iEvent.getByToken(TrackingParticleToken_, TrackingParticleHandle);

  edm::Handle<TTStubAssociationMap<Ref_Phase2TrackerDigi_>> MCTruthTTStubHandle;
  iEvent.getByToken(ttStubMCTruthToken_, MCTruthTTStubHandle);

  edm::ESHandle<TrackerTopology> tTopoHandle;
  iSetup.get<TrackerTopologyRcd>().get(tTopoHandle);

  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, coneSize_);
  std::vector<fastjet::PseudoJet> JetInputs;

  const TrackerTopology* const tTopo = tTopoHandle.product();

  int this_tp = 0;
  std::vector<TrackingParticle>::const_iterator iterTP;
  for (iterTP = TrackingParticleHandle->begin(); iterTP != TrackingParticleHandle->end(); ++iterTP) {
    edm::Ptr<TrackingParticle> tp_ptr(TrackingParticleHandle, this_tp);
    this_tp++;

    int tmp_eventid = iterTP->eventId().event();
    if (tmp_eventid > 0)
      continue;  //only care about primary interaction

    float tmp_tp_pt = iterTP->pt();
    float tmp_tp_eta = iterTP->eta();
    float tmp_tp_phi = iterTP->phi();
    float tmp_tp_vz = iterTP->vz();
    float tmp_tp_vx = iterTP->vx();
    float tmp_tp_vy = iterTP->vy();
    int tmp_tp_pdgid = iterTP->pdgId();
    float tmp_tp_z0_prod = tmp_tp_vz;
    float tmp_tp_d0_prod = tmp_tp_vx * sin(tmp_tp_phi) - tmp_tp_vy * cos(tmp_tp_phi);

    // ----------------------------------------------------------------------------------------------
    // get d0/z0 propagated back to the IP
    float tmp_tp_t = tan(2.0 * atan(1.0) - 2.0 * atan(exp(-tmp_tp_eta)));
    float delx = -tmp_tp_vx;
    float dely = -tmp_tp_vy;

    float A = 0.01 * 0.5696;
    float Kmagnitude = A / tmp_tp_pt;
    float tmp_tp_charge = tp_ptr->charge();
    float K = Kmagnitude * tmp_tp_charge;
    float d = 0;
    float tmp_tp_x0p = delx - (d + 1. / (2. * K) * sin(tmp_tp_phi));
    float tmp_tp_y0p = dely + (d + 1. / (2. * K) * cos(tmp_tp_phi));
    float tmp_tp_rp = sqrt(tmp_tp_x0p * tmp_tp_x0p + tmp_tp_y0p * tmp_tp_y0p);
    float tmp_tp_d0 = tmp_tp_charge * tmp_tp_rp - (1. / (2. * K));
    tmp_tp_d0 = tmp_tp_d0 * (-1);  //fix d0 sign
    static double pi = 4.0 * atan(1.0);
    float delphi = tmp_tp_phi - atan2(-K * tmp_tp_x0p, K * tmp_tp_y0p);
    if (delphi < -pi)
      delphi += 2.0 * pi;
    if (delphi > pi)
      delphi -= 2.0 * pi;
    float tmp_tp_z0 = tmp_tp_vz + tmp_tp_t * delphi / (2.0 * K);
    // ----------------------------------------------------------------------------------------------


    if (fabs(tmp_tp_z0) > TP_maxZ0)
      continue;
    if (fabs(tmp_tp_eta) > TP_maxEta)
      continue;
    if (tmp_tp_pt < TP_minPt)
      continue;


    std::vector<edm::Ref<edmNew::DetSetVector<TTStub<Ref_Phase2TrackerDigi_>>, TTStub<Ref_Phase2TrackerDigi_>>>
      theStubRefs = MCTruthTTStubHandle->findTTStubRefs(tp_ptr);
    int nStubTP = (int)theStubRefs.size();

    // how many layers/disks have stubs?
    int hasStubInLayer[11] = {0};
    for (auto& theStubRef : theStubRefs) {
      DetId detid(theStubRef->getDetId());

      int layer = -1;
      if (detid.subdetId() == StripSubdetector::TOB) {
        layer = static_cast<int>(tTopo->layer(detid)) - 1;  //fill in array as entries 0-5
      } else if (detid.subdetId() == StripSubdetector::TID) {
        layer = static_cast<int>(tTopo->layer(detid)) + 5;  //fill in array as entries 6-10
      }

      //treat genuine stubs separately (==2 is genuine, ==1 is not)
      if (MCTruthTTStubHandle->findTrackingParticlePtr(theStubRef).isNull() && hasStubInLayer[layer] < 2)
        hasStubInLayer[layer] = 1;
      else
        hasStubInLayer[layer] = 2;
    }

    int nStubLayerTP = 0;
    int nStubLayerTP_g = 0;
    for (int isum : hasStubInLayer) {
      if (isum >= 1)
        nStubLayerTP += 1;
      if (isum == 2)
        nStubLayerTP_g += 1;
    }

    if (DebugMode)
      edm::LogVerbatim("Tracklet") << "TP is associated with " << nStubTP << " stubs, and has stubs in " << nStubLayerTP << " different layers/disks, and has GENUINE stubs in " << nStubLayerTP_g << " layers ";

    if (TP_minNStub > 0) {
      if (DebugMode)
    	edm::LogVerbatim("Tracklet") << "Only consider TPs with >= " << TP_minNStub << " stubs";
      if (nStubTP < TP_minNStub) {
        if (DebugMode)
    	  edm::LogVerbatim("Tracklet") << "TP fails minimum nbr stubs requirement! Continuing...";
        continue;
      }
    }

    if (TP_minNStubLayer > 0) {
      if (DebugMode)
    	edm::LogVerbatim("Tracklet") << "Only consider TPs with stubs in >= " << TP_minNStubLayer << " layers/disks";
      if (nStubLayerTP < TP_minNStubLayer) {
        if (DebugMode)
    	  edm::LogVerbatim("Tracklet") << "TP fails stubs in minimum nbr of layers/disks requirement! Continuing...";
        continue;
      }
    }

    // std::cout << "CASE_1| px: " <<  iterTP->pt()*cos(iterTP->phi()) << "|py: " <<  iterTP->pt()*sin(iterTP->phi()) << "|pz: " << iterTP->pt()*sinh(iterTP->eta()) << "|mag: " << sqrt(iterTP->pt()*cos(iterTP->phi()) * iterTP->pt()*cos(iterTP->phi()) + iterTP->pt()*sin(iterTP->phi()) * iterTP->pt()*sin(iterTP->phi()) + iterTP->pt()*sinh(iterTP->eta()) * iterTP->pt()*sinh(iterTP->eta())) << std::endl; 

    // std::cout << "CASE_2| px: " <<  iterTP->momentum().x() << "|py: " << iterTP->momentum().y() << "|pz: " << iterTP->momentum().z() << "|mag2: " << iterTP->momentum().mag2() << std::endl; 
 
    fastjet::PseudoJet psuedoJet(iterTP->momentum().x(),
                                 iterTP->momentum().y(),
                                 iterTP->momentum().z(),
                                 sqrt(iterTP->momentum().mag2()));
    JetInputs.push_back(psuedoJet);                     // input tracks for clustering
    JetInputs.back().set_user_index(this_tp - 1);  // save track index in the collection

  }

  fastjet::ClusterSequence cs(JetInputs, jet_def);  // define the output jet collection
  std::vector<fastjet::PseudoJet> JetOutputs =
    fastjet::sorted_by_pt(cs.inclusive_jets(0));  // output jet collection, pT-ordered

  for (unsigned int ijet = 0; ijet < JetOutputs.size(); ++ijet) {
    math::XYZTLorentzVector jetP4(JetOutputs[ijet].px(), JetOutputs[ijet].py(), JetOutputs[ijet].pz(), JetOutputs[ijet].modp());
    float sumpt = 0;
    float avgZ = 0;
    std::vector<edm::Ptr<TrackingParticle>> L1TrackingParticlePtrs;
    std::vector<fastjet::PseudoJet> fjConstituents = fastjet::sorted_by_pt(cs.constituents(JetOutputs[ijet]));

    for (unsigned int i = 0; i < fjConstituents.size(); ++i) {
      auto index = fjConstituents[i].user_index();
      edm::Ptr<TrackingParticle> tp_ptr(TrackingParticleHandle, index); 
      L1TrackingParticlePtrs.push_back(tp_ptr);  // L1TrackingParticle in the jet
      sumpt = sumpt + tp_ptr->momentum().perp2();
      avgZ = avgZ + tp_ptr->momentum().perp2() * tp_ptr->z0();
    }
    avgZ = avgZ / sumpt;
    edm::Ref<JetBxCollection> jetRef;
    TPJet trkJet(jetP4, jetRef, L1TrackingParticlePtrs, avgZ);
    L1TrackingParticleJets->push_back(trkJet);
  }  //end loop over Jet Outputs

  if (displaced_)
    iEvent.put(std::move(L1TrackingParticleJets), "L1TrackingParticleJetsExtended");
  else
    iEvent.put(std::move(L1TrackingParticleJets), "L1TrackingParticleJets");
}


void L1TrackingParticleJetProducer::beginJob() {}

void L1TrackingParticleJetProducer::endJob() {}

void L1TrackingParticleJetProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TrackingParticleJetProducer);
