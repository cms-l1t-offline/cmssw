#ifndef L1Trigger_Phase2L1Taus_L1HPSPFTauBuilder_h
#define L1Trigger_Phase2L1Taus_L1HPSPFTauBuilder_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"                   // edm::ParameterSet
#include "DataFormats/Provenance/interface/ProductID.h"                   // edm::ProductID

#include "L1Trigger/Phase2L1Taus/interface/LocalFileInPath.h"          // LocalFileInPath
#include "L1Trigger/Phase2L1Taus/interface/L1HPSPFTauQualityCut.h" // L1HPSPFTauQualityCut
#include "DataFormats/Phase2L1Taus/interface/L1HPSPFTau.h"         // l1t::L1HPSPFTau
#include "DataFormats/Phase2L1ParticleFlow/interface/PFCandidate.h"       // l1t::PFCandidate, l1t::PFCandidateCollection, l1t::PFCandidateRef
#include "DataFormats/Phase2L1ParticleFlow/interface/PFJet.h"             // l1t::PFJet, l1t::PFJetCollection, l1t::PFJetRef
#include "DataFormats/L1TVertex/interface/Vertex.h"                       // l1t::VertexRef

#include <TFormula.h> // TFormula
#include <TFile.h>    // TFile
#include <TH1.h>      // TH1

#include <vector>

class L1HPSPFTauBuilder
{
 public:
  L1HPSPFTauBuilder(const edm::ParameterSet& cfg);
  ~L1HPSPFTauBuilder();
  
  void reset();
  void setL1PFCandProductID(const edm::ProductID& l1PFCandProductID);
  void setVertex(const l1t::VertexRef& primaryVertex);
  void setL1PFTauSeed(const l1t::PFCandidateRef& l1PFCand_seed);
  void setL1PFTauSeed(const l1t::PFJetRef& l1PFJet_seed);
  void addL1PFCandidates(const std::vector<l1t::PFCandidateRef>& l1PFCands);
  void setRho(double rho);
  void buildL1PFTau();

  l1t::L1HPSPFTau getL1PFTau() const { return l1PFTau_; }

 private:
  l1t::PFCandidateRefVector convertToRefVector(const std::vector<l1t::PFCandidateRef>& l1PFCands);

  bool isWithinSignalCone(const l1t::PFCandidate& l1PFCand);
  bool isWithinStrip(const l1t::PFCandidate& l1PFCand);
  bool isWithinIsolationCone(const l1t::PFCandidate& l1PFCand);

  TFormula* signalConeSizeFormula_;
  static int signalConeSizeFormula_instance_counter_;
  double signalConeSize_;
  double signalConeSize2_;
  double min_signalConeSize_;
  double max_signalConeSize_;

  bool useStrips_;
  double stripSize_eta_;
  double stripSize_phi_;

  double isolationConeSize_;
  double isolationConeSize2_;

  std::vector<L1HPSPFTauQualityCut> signalQualityCuts_dzCut_disabled_;
  std::vector<L1HPSPFTauQualityCut> signalQualityCuts_dzCut_enabled_primary_;
  std::vector<L1HPSPFTauQualityCut> isolationQualityCuts_dzCut_disabled_;
  std::vector<L1HPSPFTauQualityCut> isolationQualityCuts_dzCut_enabled_primary_;
  std::vector<L1HPSPFTauQualityCut> isolationQualityCuts_dzCut_enabled_pileup_;

  std::string inputFileName_rhoCorr_;
  TFile* inputFile_rhoCorr_;
  std::string histogramName_rhoCorr_;
  TH1* histogram_rhoCorr_;
  double histogram_rhoCorr_yMax_;

  edm::ProductID l1PFCandProductID_;
  bool isPFCandSeeded_;
  l1t::PFCandidateRef l1PFCand_seed_;
  bool isPFJetSeeded_;
  l1t::PFJetRef l1PFJet_seed_;
  double l1PFTauSeed_eta_;
  double l1PFTauSeed_phi_;
  double l1PFTauSeed_zVtx_;
  double sumAllL1PFCandidates_pt_;
  l1t::VertexRef primaryVertex_;
  l1t::L1HPSPFTau l1PFTau_;
  double rho_;

  reco::Particle::LorentzVector strip_p4_;

  std::vector<l1t::PFCandidateRef> signalAllL1PFCandidates_;
  std::vector<l1t::PFCandidateRef> signalChargedHadrons_;
  std::vector<l1t::PFCandidateRef> signalElectrons_;
  std::vector<l1t::PFCandidateRef> signalNeutralHadrons_;
  std::vector<l1t::PFCandidateRef> signalPhotons_;
  std::vector<l1t::PFCandidateRef> signalMuons_;
  
  std::vector<l1t::PFCandidateRef> stripAllL1PFCandidates_;
  std::vector<l1t::PFCandidateRef> stripElectrons_;
  std::vector<l1t::PFCandidateRef> stripPhotons_;

  std::vector<l1t::PFCandidateRef> isoAllL1PFCandidates_;
  std::vector<l1t::PFCandidateRef> isoChargedHadrons_;
  std::vector<l1t::PFCandidateRef> isoElectrons_;
  std::vector<l1t::PFCandidateRef> isoNeutralHadrons_;
  std::vector<l1t::PFCandidateRef> isoPhotons_;
  std::vector<l1t::PFCandidateRef> isoMuons_;
  
  std::vector<l1t::PFCandidateRef> sumAllL1PFCandidates_;
  std::vector<l1t::PFCandidateRef> sumChargedHadrons_;
  std::vector<l1t::PFCandidateRef> sumElectrons_;
  std::vector<l1t::PFCandidateRef> sumNeutralHadrons_;
  std::vector<l1t::PFCandidateRef> sumPhotons_;
  std::vector<l1t::PFCandidateRef> sumMuons_;

  double sumChargedIsoPileup_;
  double rhoCorr_;

  bool debug_;
};

#endif
