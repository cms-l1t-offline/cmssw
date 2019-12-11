#include "SimMuon/GEMDigitizer/interface/GEMPadDigiClusterProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Common/interface/Handle.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <string>
#include <map>
#include <vector>


GEMPadDigiClusterProducer::GEMPadDigiClusterProducer(const edm::ParameterSet& ps) : geometry_(nullptr) {
  pads_ = ps.getParameter<edm::InputTag>("InputCollection");
  maxClustersOHGE11_ = ps.getParameter<unsigned int>("maxClustersOHGE11");
  maxClustersOHGE21_ = ps.getParameter<unsigned int>("maxClustersOHGE21");
  nOHGE11_ = ps.getParameter<unsigned int>("nOHGE11");
  nOHGE21_ = ps.getParameter<unsigned int>("nOHGE21");
  maxClusterSize_ = ps.getParameter<unsigned int>("maxClusterSize");

  pad_token_ = consumes<GEMPadDigiCollection>(pads_);

  produces<GEMPadDigiClusterCollection>();
  consumes<GEMPadDigiCollection>(pads_);
}


GEMPadDigiClusterProducer::~GEMPadDigiClusterProducer()
{}

/*
void GEMPadDigiClusterProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("InputCollection", edm::InputTag("simMuonGEMPadDigis"));
  desc.add<unsigned int>("maxClustersOHGE11", 4);
  desc.add<unsigned int>("maxClustersOHGE21", 5);
  desc.add<unsigned int>("nOHGE11", 2);
  desc.add<unsigned int>("nOHGE21", 4);
  desc.add<unsigned int>("maxClusterSize", 8);

  descriptions.add("simMuonGEMPadDigiClustersDef", desc);
}
*/


void GEMPadDigiClusterProducer::beginRun(const edm::Run& run, const edm::EventSetup& eventSetup)
{
  edm::ESHandle<GEMGeometry> hGeom;
  eventSetup.get<MuonGeometryRecord>().get(hGeom);
  geometry_ = &*hGeom;
}


void GEMPadDigiClusterProducer::produce(edm::Event& e, const edm::EventSetup& eventSetup)
{
  edm::Handle<GEMPadDigiCollection> hpads;
  e.getByToken(pad_token_, hpads);

  // Create empty output
  std::unique_ptr<GEMPadDigiClusterCollection> pClusters(new GEMPadDigiClusterCollection());

  // build the proto clusters (per partition)
  GEMPadDigiClusterContainer proto_clusters;
  buildClusters(*(hpads.product()), proto_clusters);

  // // sort clusters per chamber, per OH, per partition number and per pad number
  GEMPadDigiClusterSortedContainer sorted_clusters;
  sortClusters(proto_clusters, sorted_clusters);

  // select the clusters from sorted clusters
  selectClusters(sorted_clusters, *pClusters);

  // store them in the event
  e.put(std::move(pClusters));
}


void GEMPadDigiClusterProducer::buildClusters(const GEMPadDigiCollection& det_pads,
                                              GEMPadDigiClusterContainer& proto_clusters) const {
  // clear the container
  proto_clusters.clear();

  // construct clusters
  for (const auto& part : geometry_->etaPartitions()) {
    GEMPadDigiClusters all_pad_clusters;

    auto pads = det_pads.get(part->id());
    std::vector<uint16_t> cl;
    int startBX = 99;

    for (auto d = pads.first; d != pads.second; ++d) {
      if (cl.empty()) {
        cl.push_back((*d).pad());
      } else {
        if ((*d).bx() == startBX and            // same bunch crossing
            (*d).pad() == cl.back() + 1         // pad difference is 1
            and cl.size() < maxClusterSize_) {  // max 8 in cluster
          cl.push_back((*d).pad());
        } else {
          // put the current cluster in the proto collection
          GEMPadDigiCluster pad_cluster(cl, startBX);

          all_pad_clusters.emplace_back(pad_cluster);

          // start a new cluster
          cl.clear();
          cl.push_back((*d).pad());
        }
      }
      startBX = (*d).bx();
    }

    // put the last cluster in the proto collection
    if (pads.first != pads.second) {
      GEMPadDigiCluster pad_cluster(cl, startBX);
      all_pad_clusters.emplace_back(pad_cluster);
    }
    proto_clusters.emplace(part->id(), all_pad_clusters);

  }  // end of partition loop
}

void GEMPadDigiClusterProducer::sortClusters(const GEMPadDigiClusterContainer& proto_clusters,
                                             GEMPadDigiClusterSortedContainer& sorted_clusters) const {
  // The sorting of the clusters favors lower eta partitions and lower pad numbers
  // By default the eta partitions are sorted by Id

  sorted_clusters.clear();

  for (const auto& ch : geometry_->chambers()) {
    // check the station number
    const int station = ch->id().station();
    const bool isGE11 = (station == 1);
    const unsigned nOH = isGE11 ? nOHGE11_ : nOHGE21_;
    const unsigned nPartOH = ch->nEtaPartitions() / nOH;

    std::vector<std::vector<std::pair<GEMDetId, GEMPadDigiClusters> > > temp_clustersCH;

    for (unsigned int iOH = 0; iOH < nOH; iOH++) {
      // all clusters for a set of eta partitions
      std::vector<std::pair<GEMDetId, GEMPadDigiClusters> > temp_clustersOH;

      // loop over the 4 or 2 eta partitions for this optohybrid
      for (unsigned iPart = 1; iPart <= nPartOH; iPart++) {
        // get the clusters for this eta partition
        const GEMDetId& partId = ch->etaPartition(iPart + iOH * nPartOH)->id();
        if (proto_clusters.find(partId) != proto_clusters.end()) {
          temp_clustersOH.emplace_back(partId, proto_clusters.at(partId));
        }
      }  // end of eta partition loop

      temp_clustersCH.emplace_back(temp_clustersOH);
    }  // end of OH loop

    sorted_clusters.emplace(ch->id(), temp_clustersCH);
  }  // end of chamber loop
}

void GEMPadDigiClusterProducer::selectClusters(const GEMPadDigiClusterSortedContainer& sorted_clusters,
                                               GEMPadDigiClusterCollection& out_clusters) const {
  // loop over chambers
  for (const auto& ch : geometry_->chambers()) {
    const int station = ch->id().station();
    const bool isGE11 = (station == 1);
    const unsigned maxClustersOH = isGE11 ? maxClustersOHGE11_ : maxClustersOHGE21_;

    // loop over the optohybrids
    for (const auto& optohybrid : sorted_clusters.at(ch->id())) {
      // at most maxClustersOH per OH!
      unsigned nClusters = 0;

      // loop over the eta partitions for this OH
      for (const auto& etapart : optohybrid) {
        const auto& detid(etapart.first);
        const auto& clusters(etapart.second);

        // pick the clusters with lowest pad number
        for (const auto& clus : clusters) {
          if (nClusters < maxClustersOH) {
            std::cout << "Adding cluster to final collection " << detid << " " << clus << std::endl;
            out_clusters.insertDigi(detid, clus);
            nClusters++;
          } else {
            std::cout << "Rejecting cluster " << detid << " " << clus << std::endl;
          }
        }  // end of cluster loop
      }    // end of eta partition loop
    }      // end of OH loop
  }        // end of chamber loop
}

DEFINE_FWK_MODULE(GEMPadDigiClusterProducer);
