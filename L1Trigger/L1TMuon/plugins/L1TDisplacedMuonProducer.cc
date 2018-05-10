// system include files
#include <memory>
#include <fstream>
#include <sstream>

// user include files
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// geometry
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"

#include "L1Trigger/L1TMuon/src/Phase2/L1TDisplacedMuonBuilder.h"
//
// class declaration
//
using namespace l1t;

class L1TDisplacedMuonProducer : public edm::EDProducer
{
public:
  explicit L1TDisplacedMuonProducer(const edm::ParameterSet&);
  ~L1TDisplacedMuonProducer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void produce(edm::Event&, const edm::EventSetup&);

  // ----------member data ---------------------------
  edm::InputTag cscCompTag_;
  edm::InputTag cscLctTag_;
  edm::InputTag padTag_;
  edm::InputTag copadTag_;
  edm::InputTag me0TriggerTag_;
  edm::InputTag me0SegmentTag_;
  edm::InputTag emtfTag_;
  edm::InputTag bmtfTag_;
  edm::InputTag muonTag_;
  edm::InputTag emtfMuonTag_;
  edm::InputTag omtfMuonTag_;
  edm::InputTag bmtfMuonTag_;

  edm::EDGetTokenT<CSCComparatorDigiCollection> comparatorToken_;
  edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> lctToken_;
  edm::EDGetTokenT<GEMPadDigiCollection> padToken_;
  edm::EDGetTokenT<GEMCoPadDigiCollection> copadToken_;
  edm::EDGetTokenT<ME0TriggerDigiCollection> me0triggerToken_;
  edm::EDGetTokenT<ME0SegmentCollection> segmentToken_;
  edm::EDGetTokenT<l1t::EMTFTrackCollection> emtfToken_;
  edm::EDGetTokenT<L1MuBMTrackCollection> bmtfToken_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> bmtfMuonToken_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> omtfMuonToken_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> emtfMuonToken_;

  edm::ParameterSet config_;
};

L1TDisplacedMuonProducer::L1TDisplacedMuonProducer(const edm::ParameterSet& iConfig)
{
  cscCompTag_ = iConfig.getParameter<edm::InputTag>("cscCompTag");
  cscLctTag_ = iConfig.getParameter<edm::InputTag>("cscLctTag");
  padTag_ = iConfig.getParameter<edm::InputTag>("padTag");
  copadTag_ = iConfig.getParameter<edm::InputTag>("copadTag");
  me0TriggerTag_ = iConfig.getParameter<edm::InputTag>("me0TriggerTag");
  me0SegmentTag_ = iConfig.getParameter<edm::InputTag>("me0SegmentTag");
  emtfTag_ = iConfig.getParameter<edm::InputTag>("emtfTag");
  bmtfTag_ = iConfig.getParameter<edm::InputTag>("bmtfTag");
  emtfMuonTag_ = iConfig.getParameter<edm::InputTag>("emtfMuonTag");
  bmtfMuonTag_ = iConfig.getParameter<edm::InputTag>("bmtfMuonTag");
  bmtfMuonTag_ = iConfig.getParameter<edm::InputTag>("bmtfMuonTag");
  comparatorToken_ = consumes<CSCComparatorDigiCollection>(cscCompTag_);
  lctToken_ = consumes<CSCCorrelatedLCTDigiCollection>(cscLctTag_);
  padToken_ = consumes<GEMPadDigiCollection>(padTag_);
  copadToken_ = consumes<GEMCoPadDigiCollection>(copadTag_);
  me0triggerToken_ = consumes<ME0TriggerDigiCollection>(me0TriggerTag_);
  segmentToken_ = consumes<ME0SegmentCollection>(me0SegmentTag_);
  emtfToken_ = consumes<l1t::EMTFTrackCollection>(emtfTag_);
  bmtfToken_ = consumes<L1MuBMTrackCollection>(bmtfTag_);
  bmtfMuonToken_ = consumes<l1t::RegionalMuonCandBxCollection>(bmtfMuonTag_);
  omtfMuonToken_ = consumes<l1t::RegionalMuonCandBxCollection>(omtfMuonTag_);
  emtfMuonToken_ = consumes<l1t::RegionalMuonCandBxCollection>(emtfMuonTag_);

  config_ = iConfig;

  //register your products
  produces<l1t::MuonBxCollection>("NoVtx");
}

L1TDisplacedMuonProducer::~L1TDisplacedMuonProducer()
{
}

void
L1TDisplacedMuonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // construct a new builder
  std::unique_ptr<L1TMuon::L1TDisplacedMuonBuilder>
    builder( new L1TMuon::L1TDisplacedMuonBuilder(config_) );

  // get the trigger geometry
  edm::ESHandle<CSCGeometry> h;
  iSetup.get<MuonGeometryRecord>().get(h);
  builder->setCSCGeometry(&*h);

  edm::ESHandle<GEMGeometry> h_gem;
  iSetup.get<MuonGeometryRecord>().get(h_gem);
  builder->setGEMGeometry(&*h_gem);

  edm::ESHandle<RPCGeometry> h_rpc;
  iSetup.get<MuonGeometryRecord>().get(h_rpc);
  builder->setRPCGeometry(&*h_rpc);

  edm::ESHandle<DTGeometry> h_dt;
  iSetup.get<MuonGeometryRecord>().get(h_dt);
  builder->setDTGeometry(&*h_dt);

  edm::ESHandle<ME0Geometry> h_me0;
  iSetup.get<MuonGeometryRecord>().get(h_me0);
  builder->setME0Geometry(&*h_me0);

  // input collections
  edm::Handle<CSCComparatorDigiCollection> comparators;
  iEvent.getByToken(comparatorToken_, comparators);

  edm::Handle<CSCCorrelatedLCTDigiCollection> lcts;
  iEvent.getByToken(lctToken_, lcts);

  edm::Handle<GEMPadDigiCollection> pads;
  iEvent.getByToken(padToken_, pads);

  edm::Handle<GEMCoPadDigiCollection> copads;
  iEvent.getByToken(copadToken_, copads);

  edm::Handle<ME0SegmentCollection> segments;
  iEvent.getByToken(segmentToken_, segments);

  edm::Handle<l1t::EMTFTrackCollection> emtfTracks;
  iEvent.getByToken(emtfToken_, emtfTracks);

  edm::Handle<L1MuBMTrackCollection> bmtfTracks;
  iEvent.getByToken(bmtfToken_, bmtfTracks);

  edm::Handle<l1t::RegionalMuonCandBxCollection> bmtfMuons;
  iEvent.getByToken(bmtfMuonToken_, bmtfMuons);

  edm::Handle<l1t::RegionalMuonCandBxCollection> omtfMuons;
  iEvent.getByToken(omtfMuonToken_, omtfMuons);

  edm::Handle<l1t::RegionalMuonCandBxCollection> emtfMuons;
  iEvent.getByToken(emtfMuonToken_, emtfMuons);

  // new output collection
  std::unique_ptr<l1t::MuonPhase2BxCollection> outMuonsPhase2 (new l1t::MuonPhase2BxCollection());

  // build the displaced muons
  builder->build(comparators.product(),
                 lcts.product(),
                 pads.product(),
                 copads.product(),
                 segments.product(),
                 emtfTracks.product(),
                 bmtfTracks.product(),
                 bmtfMuons,
                 omtfMuons,
                 emtfMuons,
                 outMuonsPhase2);

  // put output collection in event
  iEvent.put(std::move(outMuonsPhase2),"NoVtx");
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1TDisplacedMuonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TDisplacedMuonProducer);
