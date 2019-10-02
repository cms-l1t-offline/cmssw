#ifndef L1Trigger_Phase2L1Taus_L1HPSPFTauQualityCut_h
#define L1Trigger_Phase2L1Taus_L1HPSPFTauQualityCut_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"             // edm::ParameterSet
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidate.h" // l1t::PFCandidate
#include "DataFormats/L1TVertex/interface/Vertex.h"                 // l1t::Vertex

#include <string>                                                   // std::string
#include <vector>                                                   // std::vector

class L1HPSPFTauQualityCut 
{
 public:
  /// constructor
  L1HPSPFTauQualityCut(const edm::ParameterSet& cfg);

  /// destructor
  ~L1HPSPFTauQualityCut();
    
  /// returns true (false) if PFCandidate passes (fails) quality cuts
  bool operator()(const l1t::PFCandidate& pfCand, float_t primaryVertex_z) const;

  /// accessor functions
  l1t::PFCandidate::Kind pfCandType() const; 
  enum { kDisabled, kEnabled_primary, kEnabled_pileup };
  int dzCut() const;
  float_t min_pt() const;
  float_t max_dz() const;

 private:
  l1t::PFCandidate::Kind pfCandType_;

  int dzCut_; // flag to invert dz cut in order to compute charged isolation from pileup for delta-beta corrections

  float_t min_pt_;
  float_t max_dz_;

  bool debug_;
};

std::vector<L1HPSPFTauQualityCut> readL1PFTauQualityCuts(const edm::ParameterSet& cfg, const std::string& dzCut, bool debug = false);

bool isSelected(const std::vector<L1HPSPFTauQualityCut>& qualityCuts, const l1t::PFCandidate& pfCand, float_t primaryVertex_z);

#endif
