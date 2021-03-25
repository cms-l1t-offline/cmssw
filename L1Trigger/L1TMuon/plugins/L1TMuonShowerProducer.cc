// system include files
#include <memory>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/ESGetToken.h"

#include "DataFormats/L1TMuon/interface/RegionalMuonShower.h"
#include "DataFormats/L1Trigger/interface/MuonShower.h"

class L1TMuonShowerProducer : public edm::stream::EDProducer<> {
public:
  explicit L1TMuonShowerProducer(const edm::ParameterSet&);
  ~L1TMuonShowerProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;

  // ----------member data ---------------------------
  edm::InputTag showerInputTag_;
  edm::EDGetTokenT<l1t::RegionalMuonShowerBxCollection> showerInputToken_;
};

L1TMuonShowerProducer::L1TMuonShowerProducer(const edm::ParameterSet& iConfig) {
  showerInputTag_ = iConfig.getParameter<edm::InputTag>("showerInput");
  showerInputToken_ = consumes<l1t::RegionalMuonShowerBxCollection>(showerInputTag_);

  produces<l1t::MuonShowerBxCollection>();
}

L1TMuonShowerProducer::~L1TMuonShowerProducer() {}

// ------------ method called to produce the data  ------------
void L1TMuonShowerProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  std::unique_ptr<l1t::MuonShowerBxCollection> outShowers(new l1t::MuonShowerBxCollection());

  edm::Handle<l1t::RegionalMuonShowerBxCollection> emtfShowers;
  iEvent.getByToken(showerInputToken_, emtfShowers);

  // showers always arrive in-time
  outShowers->setBXRange(0, 0);

  bool hasTwoLooseInTime(false);
  bool hasOneNominalInTime(false);
  bool hasTwoLooseOutTime(false);
  bool hasOneNominalOutTime(false);

  // current GMT shower logic: "OR" all 12 sectors together for the in-time and out-of-time bits
  for (size_t i = 0; i < emtfShowers->size(0); ++i) {
    auto shower = emtfShowers->at(0, i);
    hasTwoLooseInTime |= shower.isTwoLooseInTime();
    hasTwoLooseOutTime |= shower.isTwoLooseOutTime();
    hasOneNominalInTime |= shower.isOneNominalInTime();
    hasOneNominalOutTime |= shower.isOneNominalOutTime();
  }

  l1t::MuonShower outShower;
  if (hasOneNominalInTime)
    outShower.setBitsInTime(l1t::MuonShower::kOneNominal);
  if (hasTwoLooseInTime)
    outShower.setBitsInTime(l1t::MuonShower::kTwoLoose);
  if (hasOneNominalOutTime)
    outShower.setBitsOutTime(l1t::MuonShower::kOneNominal);
  if (hasTwoLooseOutTime)
    outShower.setBitsOutTime(l1t::MuonShower::kTwoLoose);
  outShowers->push_back(0, outShower);

  iEvent.put(std::move(outShowers));
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void L1TMuonShowerProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1TMuonShowerProducer);
