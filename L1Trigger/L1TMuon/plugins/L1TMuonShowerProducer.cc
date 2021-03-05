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

using namespace l1t;

class L1TMuonShowerProducer : public edm::stream::EDProducer<> {
public:
  explicit L1TMuonShowerProducer(const edm::ParameterSet&);
  ~L1TMuonShowerProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;

  // ----------member data ---------------------------
  int m_bxMin;
  int m_bxMax;
  bool m_autoCancelMode;
  edm::InputTag m_showerInputTag;
  unsigned m_minShowers;
  edm::EDGetTokenT<l1t::RegionalMuonShowerBxCollection> m_showerInputToken;
};

L1TMuonShowerProducer::L1TMuonShowerProducer(const edm::ParameterSet& iConfig) {
  m_showerInputTag = iConfig.getParameter<edm::InputTag>("showerInput");
  m_bxMin = iConfig.getParameter<int>("bxMin");
  m_bxMax = iConfig.getParameter<int>("bxMax");
  m_showerInputToken = consumes<l1t::RegionalMuonShowerBxCollection>(m_showerInputTag);
  m_minShowers = iConfig.getParameter<unsigned>("minShowers");

  produces<MuonShowerBxCollection>();
}

L1TMuonShowerProducer::~L1TMuonShowerProducer() {}

// ------------ method called to produce the data  ------------
void L1TMuonShowerProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  std::unique_ptr<MuonShowerBxCollection> outShowers(new MuonShowerBxCollection());

  Handle<l1t::RegionalMuonShowerBxCollection> emtfShowers;
  iEvent.getByToken(m_showerInputToken, emtfShowers);

  outShowers->setBXRange(m_bxMin, m_bxMax);

  unsigned sumShowers = 0;
  for (size_t i = 0; i < emtfShowers->size(0); ++i) {
    auto shower = emtfShowers->at(0, i);
    std::cout << "L1TMuonShowerProducer :: Found RegionalMuonShower from endcap " << shower.endcap() << " sector "
              << shower.sector() << " isTwoLoose " << shower.isTwoLoose() << " isOneNominal " << shower.isOneNominal()
              << std::endl;
    if (shower.isValid()) {
      sumShowers++;
    }
  }

  // shower logic: at least one nominal shower, or a pair of loose showers
  const bool accept(sumShowers >= m_minShowers);

  if (accept) {
    std::cout << "L1TMuonShowerProducer :: new MuonShower --> bits " << sumShowers << std::endl;
    MuonShower outShower(sumShowers);
    outShowers->push_back(0, outShower);
  }
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
