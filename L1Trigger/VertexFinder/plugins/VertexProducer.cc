#include "L1Trigger/VertexFinder/interface/VertexProducer.h"

using namespace l1tVertexFinder;
using namespace std;

VertexProducer::VertexProducer(const edm::ParameterSet& iConfig)
    : l1TracksToken_(consumes<TTTrackCollectionView>(iConfig.getParameter<edm::InputTag>("l1TracksInputTag"))),
      trackerTopologyToken_(esConsumes<TrackerTopology, TrackerTopologyRcd>()),
      outputCollectionName_(iConfig.getParameter<std::string>("l1VertexCollectionName")),
      settings_(AlgoSettings(iConfig)) {
  // Get configuration parameters

  switch (settings_.vx_algo()) {
    case Algorithm::FastHisto:
      edm::LogInfo("VertexProducer") << "VertexProducer::Finding vertices using the FastHisto binning algorithm";
      break;
    case Algorithm::FastHistoEmulation:
      edm::LogInfo("VertexProducer")
          << "VertexProducer::Finding vertices using the emulation version of the FastHisto binning algorithm";
      break;
    case Algorithm::FastHistoLooseAssociation:
      edm::LogInfo("VertexProducer")
          << "VertexProducer::Finding vertices using the FastHistoLooseAssociation binning algorithm";
      break;
    case Algorithm::GapClustering:
      edm::LogInfo("VertexProducer") << "VertexProducer::Finding vertices using a gap clustering algorithm";
      break;
    case Algorithm::AgglomerativeHierarchical:
      edm::LogInfo("VertexProducer") << "VertexProducer::Finding vertices using a Simple Merge Clustering algorithm";
      break;
    case Algorithm::DBSCAN:
      edm::LogInfo("VertexProducer") << "VertexProducer::Finding vertices using a DBSCAN algorithm";
      break;
    case Algorithm::PVR:
      edm::LogInfo("VertexProducer") << "VertexProducer::Finding vertices using a PVR algorithm";
      break;
    case Algorithm::AdaptiveVertexReconstruction:
      edm::LogInfo("VertexProducer")
          << "VertexProducer::Finding vertices using an AdaptiveVertexReconstruction algorithm";
      break;
    case Algorithm::HPV:
      edm::LogInfo("VertexProducer") << "VertexProducer::Finding vertices using a Highest Pt Vertex algorithm";
      break;
    case Algorithm::Kmeans:
      edm::LogInfo("VertexProducer") << "VertexProducer::Finding vertices using a kmeans algorithm";
      break;
  }

  // Tame debug printout.
  cout.setf(ios::fixed, ios::floatfield);
  cout.precision(4);

  //--- Define EDM output to be written to file (if required)
  if (settings_.vx_algo() == Algorithm::FastHistoEmulation) {
    produces<l1t::VertexWordCollection>(outputCollectionName_ + "Emulation");
  } else {
    produces<l1t::VertexCollection>(outputCollectionName_);
  }
}

void VertexProducer::produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const {
  edm::Handle<TTTrackCollectionView> l1TracksHandle;
  iEvent.getByToken(l1TracksToken_, l1TracksHandle);

  std::vector<l1tVertexFinder::L1Track> l1Tracks;
  l1Tracks.reserve(l1TracksHandle->size());
  if (settings_.debug() > 1) {
    edm::LogInfo("VertexProducer") << "produce::Processing " << l1TracksHandle->size() << " tracks";
  }
  for (const auto& track : l1TracksHandle->ptrs()) {
    auto l1track = L1Track(track);
    // Check the minimum pT of the tracks
    // This is left here because it represents the smallest pT to be sent by the track finding boards
    // This has less to do with the algorithms than the constraints of what will be sent to the vertexing algorithm
    if (l1track.pt() > settings_.vx_TrackMinPt()) {
      l1Tracks.push_back(l1track);
    }
  }

  VertexFinder vf(l1Tracks, settings_);

  switch (settings_.vx_algo()) {
    case Algorithm::FastHisto: {
      edm::ESHandle<TrackerTopology> tTopoHandle = iSetup.getHandle(trackerTopologyToken_);
      vf.FastHisto(tTopoHandle.product());
      break;
    }
    case Algorithm::FastHistoEmulation:
      vf.FastHistoEmulation();
      break;
    case Algorithm::FastHistoLooseAssociation:
      vf.FastHistoLooseAssociation();
      break;
    case Algorithm::GapClustering:
      vf.GapClustering();
      break;
    case Algorithm::AgglomerativeHierarchical:
      vf.AgglomerativeHierarchicalClustering();
      break;
    case Algorithm::DBSCAN:
      vf.DBSCAN();
      break;
    case Algorithm::PVR:
      vf.PVR();
      break;
    case Algorithm::AdaptiveVertexReconstruction:
      vf.AdaptiveVertexReconstruction();
      break;
    case Algorithm::HPV:
      vf.HPV();
      break;
    case Algorithm::Kmeans:
      vf.Kmeans();
      break;
  }

  vf.SortVerticesInPt();
  vf.FindPrimaryVertex();

  // //=== Store output EDM track and hardware stub collections.
  if (settings_.vx_algo() == Algorithm::FastHistoEmulation) {
    std::unique_ptr<l1t::VertexWordCollection> product_emulation =
        std::make_unique<l1t::VertexWordCollection>(vf.verticesEmulation().begin(), vf.verticesEmulation().end());
    iEvent.put(std::move(product_emulation), outputCollectionName_ + "Emulation");
  } else {
    std::unique_ptr<l1t::VertexCollection> product(new std::vector<l1t::Vertex>());
    for (const auto& vtx : vf.vertices()) {
      product->emplace_back(vtx.vertex());
    }
    iEvent.put(std::move(product), outputCollectionName_);
  }
}

DEFINE_FWK_MODULE(VertexProducer);
