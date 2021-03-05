#ifndef L1TMuonEndCap_L1TMuonEndCapShowerProducer_h
#define L1TMuonEndCap_L1TMuonEndCapShowerProducer_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/L1TMuonEndCap/interface/Common.h"
#include "L1Trigger/L1TMuonEndCap/interface/EMTFSetup.h"
#include "L1Trigger/L1TMuonEndCap/interface/EMTFSubsystemCollector.h"
#include "L1Trigger/L1TMuonEndCap/interface/SectorProcessorShower.h"

// Class declaration
class L1TMuonEndCapShowerProducer : public edm::stream::EDProducer<> {
public:
  explicit L1TMuonEndCapShowerProducer(const edm::ParameterSet&);
  ~L1TMuonEndCapShowerProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;

private:
  const edm::ParameterSet& config_;
  int verbose_;
  const edm::EDGetToken tokenCSCShower_;
  emtf::sector_array<SectorProcessorShower> sector_processors_shower_;
};

#endif
