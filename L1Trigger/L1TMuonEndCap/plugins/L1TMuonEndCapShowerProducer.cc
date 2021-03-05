#include "L1Trigger/L1TMuonEndCap/plugins/L1TMuonEndCapShowerProducer.h"

L1TMuonEndCapShowerProducer::L1TMuonEndCapShowerProducer(const edm::ParameterSet& iConfig)
    : config_(iConfig),
      verbose_(iConfig.getUntrackedParameter<int>("verbosity")),
      tokenCSCShower_(consumes<CSCShowerDigiCollection>(iConfig.getParameter<edm::InputTag>("CSCShowerInput"))),
      sector_processors_shower_() {
  // Make output products
  produces<l1t::RegionalMuonShowerBxCollection>("EMTF");
}

L1TMuonEndCapShowerProducer::~L1TMuonEndCapShowerProducer() {}

void L1TMuonEndCapShowerProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // Create pointers to output products
  auto out_showers = std::make_unique<l1t::RegionalMuonShowerBxCollection>();
  out_showers->clear();
  out_showers->setBXRange(0, 0);

  edm::Handle<CSCShowerDigiCollection> showersH;
  iEvent.getByToken(tokenCSCShower_, showersH);
  const CSCShowerDigiCollection& showers = *showersH.product();

  // ___________________________________________________________________________
  // Run the sector processors

  for (int endcap = emtf::MIN_ENDCAP; endcap <= emtf::MAX_ENDCAP; ++endcap) {
    for (int sector = emtf::MIN_TRIGSECTOR; sector <= emtf::MAX_TRIGSECTOR; ++sector) {
      const int es = (endcap - emtf::MIN_ENDCAP) * (emtf::MAX_TRIGSECTOR - emtf::MIN_TRIGSECTOR + 1) +
                     (sector - emtf::MIN_TRIGSECTOR);

      sector_processors_shower_.at(es).configure(config_, verbose_, endcap, sector);
      sector_processors_shower_.at(es).process(showers, *out_showers);
    }
  }

  // Fill the output products
  iEvent.put(std::move(out_showers), "EMTF");
}

// Fill 'descriptions' with the allowed parameters
void L1TMuonEndCapShowerProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// Define this as a plug-in
DEFINE_FWK_MODULE(L1TMuonEndCapShowerProducer);
