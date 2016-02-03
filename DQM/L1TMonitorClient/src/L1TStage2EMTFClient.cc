#include "DQM/L1TMonitorClient/interface/L1TStage2EMTFClient.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"


L1TStage2EMTFClient::L1TStage2EMTFClient(const edm::ParameterSet& ps) 
    : inputDir(ps.getUntrackedParameter<std::string>("inputDataDir", "")),
      outputDir(ps.getUntrackedParameter<std::string>("outputDir", "")) {}

L1TStage2EMTFClient::~L1TStage2EMTFClient() {}

void L1TStage2EMTFClient::dqmEndJob(DQMStore::IBooker& ibooker, DQMStore::IGetter& igetter) {
  ibooker.setCurrentFolder(outputDir);
  processHistograms(igetter);
}

void L1TStage2EMTFClient::dqmEndLuminosityBlock(DQMStore::IBooker& ibooker, DQMStore::IGetter& igetter, const edm::LuminosityBlock& lumiSeg, const edm::EventSetup& c) {
  ibooker.setCurrentFolder(outputDir);
  processHistograms(igetter);
}

void L1TStage2EMTFClient::processHistograms(DQMStore::IGetter& igetter) {
  igetter.setCurrentFolder(inputDir);

  std::vector<std::string> MEs = igetter.getMEs();
  for (std::vector<std::string>::const_iterator it = MEs.begin(); it != MEs.end(); ++it) {
    std::string fullPath = inputDir + "/" + (*it);
    MonitorElement* me = igetter.get(fullPath);
    if (!me) {
      edm::LogInfo("L1TStage2EMTFClient") << fullPath << " NOT FOUND." << std::endl;
      continue;
    }
  }
}

