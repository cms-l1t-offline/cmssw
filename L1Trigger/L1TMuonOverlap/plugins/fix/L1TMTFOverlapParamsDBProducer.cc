#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "L1Trigger/L1TMuonOverlap/plugins/L1TMTFOverlapParamsDBProducer.h"

#include "CondFormats/DataRecord/interface/L1TMuonOverlapParamsRcd.h"
#include "CondFormats/L1TObjects/interface/L1TMuonOverlapParams.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

L1MTFOverlapParamsDBProducer::L1MTFOverlapParamsDBProducer(const edm::ParameterSet & cfg){ }
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void L1MTFOverlapParamsDBProducer::beginRun(edm::Run const& run, edm::EventSetup const& iSetup){

  const L1TMuonOverlapParamsRcd& omtfParamsRcd = iSetup.get<L1TMuonOverlapParamsRcd>();
  
  edm::ESHandle<L1TMuonOverlapParams> omtfParamsHandle;
  omtfParamsRcd.get(omtfParamsHandle);

  omtfParams = std::unique_ptr<L1TMuonOverlapParams>(new L1TMuonOverlapParams(*omtfParamsHandle.product()));
  if (!omtfParams) {
    edm::LogError("L1TMuonOverlapTrackProducer") << "Could not retrieve parameters from Event Setup" << std::endl;
  }  
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void L1MTFOverlapParamsDBProducer::analyze(const edm::Event& ev, const edm::EventSetup& es){

  std::string recordName = "L1TMuonOverlapParamsRcd";
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  if(poolDbService.isAvailable()) poolDbService->writeOne(omtfParams.get(), poolDbService->currentTime(),recordName);  
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
