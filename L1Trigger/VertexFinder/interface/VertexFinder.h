#ifndef __L1Trigger_VertexFinder_VertexFinder_h__
#define __L1Trigger_VertexFinder_VertexFinder_h__

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/L1Trigger/interface/VertexWord.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/VertexFinder/interface/AlgoSettings.h"
#include "L1Trigger/VertexFinder/interface/RecoVertex.h"

#include <algorithm>
#include <iterator>
#include <vector>

namespace l1tVertexFinder {

  // Returns the number of bits needed to represent and integer decimal
  static constexpr unsigned BitsToRepresent(unsigned x) { return x < 2 ? 1 : 1 + BitsToRepresent(x >> 1); }

  typedef std::vector<L1Track> FitTrackCollection;
  typedef std::vector<RecoVertex<>> RecoVertexCollection;

  class VertexFinder {
  public:
    /// Constructor and destructor
    VertexFinder(FitTrackCollection& fitTracks, const AlgoSettings& settings) {
      fitTracks_ = fitTracks;
      settings_ = &settings;
    }
    ~VertexFinder() {}

    /// Helper structs/classes
    struct SortTracksByZ0 {
      inline bool operator()(const L1Track track0, const L1Track track1) { return (track0.z0() < track1.z0()); }
    };

    struct SortTracksByPt {
      inline bool operator()(const L1Track track0, const L1Track track1) {
        return (std::abs(track0.pt()) > std::abs(track1.pt()));
      }
    };

    /// Accessors

    /// Storage for tracks out of the L1 Track finder
    const FitTrackCollection& fitTracks() const { return fitTracks_; }
    /// Number of iterations
    unsigned int iterationsPerTrack() const { return double(iterations_) / double(fitTracks_.size()); }
    /// Storage for tracks out of the L1 Track finder
    unsigned int numInputTracks() const { return fitTracks_.size(); }
    /// Number of iterations
    unsigned int numIterations() const { return iterations_; }
    /// Number of reconstructed vertices
    unsigned int numVertices() const { return vertices_.size(); }
    /// Number of emulation vertices
    unsigned int numVerticesEmulation() const { return verticesEmulation_.size(); }
    /// Reconstructed primary vertex
    template <typename T>
    T PrimaryVertex() const {
      if ((settings_->vx_precision() == Precision::Simulation) && (pvIndex_ < vertices_.size()))
        return vertices_[pvIndex_];
      else if ((settings_->vx_precision() == Precision::Emulation) && (pvIndex_ < vertices_.size()))
        return verticesEmulation_[pvIndex_];
      else {
        edm::LogWarning("VertexFinder") << "PrimaryVertex::No primary vertex has been found.";
        return RecoVertex<>();
      }
    }
    /// Reconstructed Primary Vertex Id
    unsigned int PrimaryVertexId() const { return pvIndex_; }
    /// Returns the reconstructed primary vertices
    const RecoVertexCollection& vertices() const { return vertices_; }
    /// Returns the emulation primary vertices
    const l1t::VertexWordCollection& verticesEmulation() const { return verticesEmulation_; }

    /// Utility member functions

    /// Associate the primary vertex with the real one
    void AssociatePrimaryVertex(double trueZ0);
    /// Find the primary vertex
    void FindPrimaryVertex();
    /// Print an ASCII histogram
    template <class data_type, typename stream_type = std::ostream>
    void printHistogram(stream_type& stream,
                        std::vector<data_type> data,
                        int width = 80,
                        int minimum = 0,
                        int maximum = -1,
                        std::string title = "",
                        std::string color = "");
    /// Sort vertices in pT
    void SortVerticesInPt();
    /// Sort vertices in z
    void SortVerticesInZ0();
    /// Generate sequence of floats in a certain range
    template <typename ForwardIterator, typename T>
    void strided_iota(ForwardIterator first, ForwardIterator last, T value, T stride) {
      while (first != last) {
        *first++ = value;
        value += stride;
      }
    }

    /// Vertexing algorithms

    /// Adaptive Vertex Reconstruction algorithm
    void AdaptiveVertexReconstruction();
    /// Simple Merge Algorithm
    void AgglomerativeHierarchicalClustering();
    /// Find distance between centres of two clusters
    float CentralDistance(RecoVertex<> cluster0, RecoVertex<> cluster1);
    /// Compute the vertex parameters
    void computeAndSetVertexParameters(RecoVertex<>& vertex,
                                       const std::vector<float>& bin_centers,
                                       const std::vector<unsigned int>& counts);
    /// DBSCAN algorithm
    void DBSCAN();
    /// Histogramming algorithm
    void FastHisto(const TrackerTopology* tTopo);
    /// Histogramming algorithm (emulation)
    void FastHistoEmulation();
    /// TDR histogramming algorithmn
    void FastHistoLooseAssociation();
    /// Gap Clustering Algorithm
    void GapClustering();
    /// High pT Vertex Algorithm
    void HPV();
    /// Kmeans Algorithm
    void Kmeans();
    /// Find maximum distance in two clusters of tracks
    float MaxDistance(RecoVertex<> cluster0, RecoVertex<> cluster1);
    /// Find minimum distance in two clusters of tracks
    float MinDistance(RecoVertex<> cluster0, RecoVertex<> cluster1);
    /// Find average distance in two clusters of tracks
    float MeanDistance(RecoVertex<> cluster0, RecoVertex<> cluster1);
    /// Principal Vertex Reconstructor algorithm
    void PVR();

  private:
    const AlgoSettings* settings_;
    RecoVertexCollection vertices_;
    l1t::VertexWordCollection verticesEmulation_;
    FitTrackCollection fitTracks_;
    unsigned int pvIndex_;
    unsigned int iterations_;
  };

}  // end namespace l1tVertexFinder

#endif
