#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"

#include "DQM/L1TMonitor/interface/L1TStage2EMTF.h"


L1TStage2EMTF::L1TStage2EMTF(const edm::ParameterSet& ps) 
    : stage2EMTFTracksToken(consumes<l1t::RegionalMuonCandBxCollection>(ps.getParameter<edm::InputTag>("stage2EMTFTrackSource"))),
      outputDir(ps.getUntrackedParameter<std::string>("outputDir", "")),
      verbose(ps.getUntrackedParameter<bool>("verbose", false)) {}

L1TStage2EMTF::~L1TStage2EMTF() {}

void L1TStage2EMTF::dqmBeginRun(const edm::Run&, const edm::EventSetup&) {}

void L1TStage2EMTF::beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) {}

void L1TStage2EMTF::bookHistograms(DQMStore::IBooker& ibooker, const edm::Run&, const edm::EventSetup&) {

  ibooker.setCurrentFolder(outputDir);

  stage2EMTFnTracks = ibooker.book1D("nTracks", "Number of CSC Tracks per Event", 5, 0, 5);
  for (int bin = 1, label = 0; bin < 6; ++bin, ++label) {
    stage2EMTFnTracks->setBinLabel(bin, std::to_string(label), 1);
  }

  stage2EMTFTrackPt = ibooker.book1D("TrackPt", "EMTF Track p_{T}", 100, -200, 200);
  stage2EMTFTrackPt->setAxisTitle("Track p_{T} [GeV]", 1);

  stage2EMTFTrackEta = ibooker.book1D("TrackEta", "EMTF Track #eta", 100, -200, 200);
  stage2EMTFTrackEta->setAxisTitle("Track #eta", 1);

  stage2EMTFTrackPhi = ibooker.book1D("TrackPhi", "EMTF Track #phi", 100, -200, 200);
  stage2EMTFTrackPhi->setAxisTitle("Track #phi", 1);

  stage2EMTFTrackBX = ibooker.book2D("TrackBX", "EMTF Track Bunch Crossings", 12, 0, 12, 7, -3, 3);
  stage2EMTFTrackBX->setAxisTitle("Sector (Endcap)", 1);
  for (int bin = 1; bin < 13; ++bin) {
    if (bin > 6) {
      stage2EMTFTrackBX->setBinLabel(bin, std::to_string(bin - 6) + " (-)", 1); 
    } else {
      stage2EMTFTrackBX->setBinLabel(bin, std::to_string(bin) + " (+)", 1);
    }
  }
  stage2EMTFTrackBX->setAxisTitle("EMTF BX", 2);
  for (int bin = 1, label = -3; bin < 8; ++bin, ++label) {
    if (label > 0) {
      stage2EMTFTrackBX->setBinLabel(bin, "+" + std::to_string(label), 2);
    } else {
      stage2EMTFTrackBX->setBinLabel(bin, std::to_string(label), 2);
    }
  }

  stage2EMTFTrackOccupancy = ibooker.book2D("TrackOccupancy", "EMTF Track Occupancy", 100, -200, 200, 100, -200, 200);
  stage2EMTFTrackOccupancy->setAxisTitle("#eta", 1);
  stage2EMTFTrackOccupancy->setAxisTitle("#phi", 2);
}

void L1TStage2EMTF::analyze(const edm::Event& e, const edm::EventSetup& c) {

  if (verbose) edm::LogInfo("L1TStage2EMTF") << "L1TStage2EMTF: analyze..." << std::endl;

  int nTracks = 0;

  edm::Handle<l1t::RegionalMuonCandBxCollection> Tracks;
  e.getByToken(stage2EMTFTracksToken, Tracks);
 
  for (int itBX = Tracks->getFirstBX(); itBX <= Tracks->getLastBX(); ++itBX) {
    for (l1t::RegionalMuonCandBxCollection::const_iterator track = Tracks->begin(itBX); track != Tracks->end(itBX); ++track) {
        
      l1t::tftype tfType = track->trackFinderType();

      if (tfType == l1t::emtf_pos) {
        stage2EMTFTrackBX->Fill(track->processor(), itBX);
      } else if (tfType == l1t::emtf_neg) {
        stage2EMTFTrackBX->Fill(track->processor() + 6, itBX);
      }

      stage2EMTFTrackPt->Fill(track->hwPt());
      stage2EMTFTrackEta->Fill(track->hwEta());
      stage2EMTFTrackPhi->Fill(track->hwPhi());
        
      nTracks++;
    }
  }  
  stage2EMTFnTracks->Fill(nTracks);
}

