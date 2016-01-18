#ifndef L1MTFOverlapParamsDBProducer_H
#define L1MTFOverlapParamsDBProducer_H

#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class L1TMuonOverlapParams;

class L1MTFOverlapParamsDBProducer : public edm::EDAnalyzer {
public:
  L1MTFOverlapParamsDBProducer(const edm::ParameterSet & cfg);
  virtual ~L1MTFOverlapParamsDBProducer(){}
  virtual void beginJob(){};
  virtual void beginRun(const edm::Run&,  const edm::EventSetup& es);
  virtual void analyze(const edm::Event&, const edm::EventSetup& es);
  virtual void endJob(){};

private:

  std::unique_ptr<L1TMuonOverlapParams> omtfParams;

}; 

#endif
