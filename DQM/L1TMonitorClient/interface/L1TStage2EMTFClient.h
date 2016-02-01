#ifndef DQM_L1TMonitorClient_L1TStage2EMTFClient_h
#define DQM_L1TMonitorClient_L1TStage2EMTFClient_h

#include "DQMServices/Core/interface/DQMEDHarvester.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


class L1TStage2EMTFClient: public DQMEDHarvester {

 public:

  L1TStage2EMTFClient(const edm::ParameterSet& ps);
  virtual ~L1TStage2EMTFClient();

 protected:

  void dqmEndJob(DQMStore::IBooker& ibooker, DQMStore::IGetter& igetter) override;
  void dqmEndLuminosityBlock(DQMStore::IBooker& ibooker, DQMStore::IGetter& igetter, const edm::LuminosityBlock& lumiSeg, const edm::EventSetup& c);

 private:

  void processHistograms(DQMStore::IGetter& igetter);

  std::string inputDir, outputDir;
};

#endif
