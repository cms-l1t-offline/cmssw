#ifndef SimMuon_GEMDigitizer_GEMPadDigiClusterProducer_h
#define SimMuon_GEMDigitizer_GEMPadDigiClusterProducer_h

/**
 *  \class ME0PadDigiClusterProducer
 *
 *  Produces GEM pad clusters from at most 8 adjacent GEM pads.
 *  Clusters are used downstream in the CSC local trigger to build
 *  GEM-CSC triggers and in the muon trigger to build EMTF tracks
 *
 *  Based on documentation provided by the GEM firmware architects
 *
 *  \author Sven Dildick (TAMU)
 */

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/GEMDigi/interface/GEMPadDigiCollection.h"
#include "DataFormats/GEMDigi/interface/GEMPadDigiClusterCollection.h"

#include <string>
#include <map>
#include <vector>

class GEMPadDigiClusterProducer : public edm::stream::EDProducer<> {
 public:
  // all clusters per eta partition
  typedef std::vector<GEMPadDigiCluster> GEMPadDigiClusters;
  typedef std::map<GEMDetId, GEMPadDigiClusters> GEMPadDigiClusterContainer;

  // all clusters sorted by chamber, by opthohybrid and by eta partition
  typedef std::map<GEMDetId, std::vector<std::vector<std::pair<GEMDetId, GEMPadDigiClusters> > > >
    GEMPadDigiClusterSortedContainer;

  explicit GEMPadDigiClusterProducer(const edm::ParameterSet& ps);

  ~GEMPadDigiClusterProducer() override;

  void beginRun(const edm::Run&, const edm::EventSetup&) override;

  void produce(edm::Event&, const edm::EventSetup&) override;

  /* static void fillDescriptions(edm::ConfigurationDescriptions& descriptions); */

 private:
  /**
   *
   *************************************
   ** Light Cluster Packing Algorithm **
   *************************************
   Based on: https://github.com/cms-gem-daq-project/OptoHybridv3/raw/master/doc/OH_modules.docx
   (Andrew Peck, Thomas Lenzi, Evaldas Juska)
   In the current version of the algorithm, cluster finding is segmented
   into two separate halves of the GE1/1 chambers. Thus, each one of the
   trigger fibers can transmit clusters only from the half of the chamber
   that it corresponds to. For GE2/1, there are four separate quarts of
   the GE2/1 chamber.
   This has the downside of being unable to transmit more than 4 clusters
   when they occur within that side of the chamber, so there will be a
   slightly higher rate of cluster overflow. For GE2/1 each OH can transmit
   up to 5 clusters.
   The benefit, however, is in terms of (1) latency and (2) resource usage.
   The burden of finding clusters on  of the chamber is significantly less,
   and allows the cluster packer to operate in a simple, pipelined architecture
   which returns up to 4 (or 5) clusters per half-chamber per bunch crossing.
   This faster architecture allows the mechanism to operate with only a
   single copy of the cluster finding priority encoder and cluster truncator
   (instead of two multiplexed copies), so the total resource usage of
   these stages is approximately half.
   Further, a second step of cluster merging that is required in the full
   algorithm is avoided, which reduces latency by an additional bunch
   crossing and significantly reduces resource usage as well.
   The sorting of the clusters favors lower eta partitions and lower pad numbers
  */

  void buildClusters(const GEMPadDigiCollection& pads, GEMPadDigiClusterContainer& out_clusters) const;

  void sortClusters(const GEMPadDigiClusterContainer& in_clusters,
                    GEMPadDigiClusterSortedContainer& out_clusters) const;

                    void selectClusters(const GEMPadDigiClusterSortedContainer& in, GEMPadDigiClusterCollection& out) const;

                    /// Name of input digi Collection
                    edm::EDGetTokenT<GEMPadDigiCollection> pad_token_;
                    edm::InputTag pads_;

                    unsigned int maxClustersOHGE11_;
                    unsigned int maxClustersOHGE21_;
                    unsigned int nOHGE11_;
                    unsigned int nOHGE21_;
                    unsigned int maxClusterSize_;

                    const GEMGeometry* geometry_;
                    };

#endif
