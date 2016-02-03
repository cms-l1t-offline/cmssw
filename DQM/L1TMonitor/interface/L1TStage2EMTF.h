#ifndef DQM_L1TMonitor_L1TStage2EMTF_h
#define DQM_L1TMonitor_L1TStage2EMTF_h

#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class L1TStage2EMTF : public DQMEDAnalyzer {

 public:

  L1TStage2EMTF(const edm::ParameterSet& ps);
  virtual ~L1TStage2EMTF();

 protected:

  virtual void dqmBeginRun(const edm::Run&, const edm::EventSetup&);
  virtual void beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);
  virtual void bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event& e, const edm::EventSetup& c);

 private:

  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> stage2EMTFTracksToken;
  std::string outputDir;
  bool verbose;
 
  MonitorElement* stage2EMTFnTracks;
  MonitorElement* stage2EMTFTrackPt;
  MonitorElement* stage2EMTFTrackEta;
  MonitorElement* stage2EMTFTrackPhi;
  MonitorElement* stage2EMTFTrackBX;
  MonitorElement* stage2EMTFTrackOccupancy;
};

#endif
