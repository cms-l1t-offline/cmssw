#include "L1Trigger/Phase2L1Taus/interface/L1HPSPFTauQualityCut.h"

#include "FWCore/Utilities/interface/Exception.h" // cms::Exception

L1HPSPFTauQualityCut::L1HPSPFTauQualityCut(const edm::ParameterSet& cfg)
  : debug_(cfg.getUntrackedParameter<bool>("debug", false))
{
  std::string pfCandType_string = cfg.getParameter<std::string>("pfCandType");
  if      ( pfCandType_string == "chargedHadron" ) pfCandType_ = l1t::PFCandidate::ChargedHadron;
  else if ( pfCandType_string == "electron"      ) pfCandType_ = l1t::PFCandidate::Electron;
  else if ( pfCandType_string == "muon"          ) pfCandType_ = l1t::PFCandidate::Muon;
  else if ( pfCandType_string == "neutralHadron" ) pfCandType_ = l1t::PFCandidate::NeutralHadron;
  else if ( pfCandType_string == "photon"        ) pfCandType_ = l1t::PFCandidate::Photon;
  else throw cms::Exception("L1HPSPFTauQualityCut")
    << "Invalid Configuration parameter 'pfCandType' = '" << pfCandType_string << "' !!\n";

  std::string dzCut_string = cfg.getParameter<std::string>("dzCut");
  if      ( dzCut_string == "disabled"        ) dzCut_ = kDisabled;
  else if ( dzCut_string == "enabled_primary" ) dzCut_ = kEnabled_primary;
  else if ( dzCut_string == "enabled_pileup"  ) dzCut_ = kEnabled_pileup;
  else throw cms::Exception("L1HPSPFTauQualityCut")
    << "Invalid Configuration parameter 'dzCut' = '" << dzCut_string << "' !!\n";

  min_pt_ = cfg.getParameter<double>("min_pt");
  max_dz_ = ( cfg.exists("max_dz") ) ? cfg.getParameter<double>("max_dz") : 1.e+3;

  if ( debug_ && dzCut_ == kEnabled_primary )
  {
    std::cout << " applying pT > " << min_pt_ << " GeV && dz < " << max_dz_ << " cm to PFCands of type = '" << pfCandType_string << "'" << std::endl; 
  } 
}

L1HPSPFTauQualityCut::~L1HPSPFTauQualityCut()
{}

bool L1HPSPFTauQualityCut::operator()(const l1t::PFCandidate& pfCand, float_t primaryVertex_z) const
{
  if ( pfCand.id() == pfCandType_ )
  {  
    if ( pfCand.pt() < min_pt_ ) 
    {
      return false;
    }

    if ( pfCand.charge() != 0 )
    {
      if ( dzCut_ == kEnabled_primary || dzCut_ == kEnabled_pileup )
      {
	l1t::PFTrackRef pfCand_track = pfCand.pfTrack();
	double dz = std::fabs(pfCand_track->vertex().z() - primaryVertex_z);  
	if ( dzCut_ == kEnabled_primary && dz >  max_dz_ ) return false;
	if ( dzCut_ == kEnabled_pileup  && dz <= max_dz_ ) return false;
      }
    }
    else if ( dzCut_ == kEnabled_pileup )
    {
      return false; // CV: only consider charged PFCands as originating from pileup
    }
  }
  return true;
}

l1t::PFCandidate::Kind L1HPSPFTauQualityCut::pfCandType() const
{
  return pfCandType_;
}

int L1HPSPFTauQualityCut::dzCut() const
{
  return dzCut_;
}
 
float_t L1HPSPFTauQualityCut::min_pt() const
{
  return min_pt_;
}

float_t L1HPSPFTauQualityCut::max_dz() const
{
  return max_dz_;
}

L1HPSPFTauQualityCut readL1PFTauQualityCut(const edm::ParameterSet& cfg, const std::string& pfCandType, const std::string& dzCut, bool debug)
{
  edm::ParameterSet cfg_pfCandType = cfg.getParameter<edm::ParameterSet>(pfCandType);
  cfg_pfCandType.addParameter<std::string>("pfCandType", pfCandType);
  cfg_pfCandType.addParameter<std::string>("dzCut", dzCut);
  cfg_pfCandType.addUntrackedParameter<bool>("debug", debug);
  L1HPSPFTauQualityCut qualityCut(cfg_pfCandType);
  return qualityCut;
}

std::vector<L1HPSPFTauQualityCut> readL1PFTauQualityCuts(const edm::ParameterSet& cfg, const std::string& dzCut, bool debug)
{
  std::vector<L1HPSPFTauQualityCut> qualityCuts;
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "chargedHadron", dzCut, debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "electron",      dzCut, debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "muon",          dzCut, debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "photon",        dzCut, debug));
  qualityCuts.push_back(readL1PFTauQualityCut(cfg, "neutralHadron", dzCut, debug));
  return qualityCuts;
}

bool isSelected(const std::vector<L1HPSPFTauQualityCut>& qualityCuts, const l1t::PFCandidate& pfCand, float_t primaryVertex_z)
{
  for ( auto qualityCut : qualityCuts ) 
  {
    if ( !qualityCut(pfCand, primaryVertex_z) ) return false;
  }
  return true;
}
