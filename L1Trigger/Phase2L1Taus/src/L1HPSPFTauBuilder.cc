#include "L1Trigger/Phase2L1Taus/interface/L1HPSPFTauBuilder.h"

#include "FWCore/Utilities/interface/Exception.h"                // cms::Exception

#include "DataFormats/Math/interface/deltaR.h"                   // reco::deltaR

#include "L1Trigger/Phase2L1Taus/interface/lutAuxFunctions.h" // openFile(), loadTH1()

#include <TString.h> // TString, Form()
#include <TMath.h>   // TMath::Pi()

#include <string>    // std::string 
#include <algorithm> // std::max(), std::sort()
#include <cmath>     // std::fabs

int L1HPSPFTauBuilder::signalConeSizeFormula_instance_counter_ = 0;

L1HPSPFTauBuilder::L1HPSPFTauBuilder(const edm::ParameterSet& cfg)
  : signalConeSizeFormula_(nullptr)
  , min_signalConeSize_(cfg.getParameter<double>("min_signalConeSize"))
  , max_signalConeSize_(cfg.getParameter<double>("max_signalConeSize"))
  , useStrips_(cfg.getParameter<bool>("useStrips"))
  , stripSize_eta_(cfg.getParameter<double>("stripSize_eta"))
  , stripSize_phi_(cfg.getParameter<double>("stripSize_phi"))
  , isolationConeSize_(cfg.getParameter<double>("isolationConeSize"))
  , inputFileName_rhoCorr_(cfg.getParameter<std::string>("inputFileName_rhoCorr"))
  , inputFile_rhoCorr_(nullptr)
  , histogramName_rhoCorr_(cfg.getParameter<std::string>("histogramName_rhoCorr"))
  , histogram_rhoCorr_(nullptr)  
  , histogram_rhoCorr_yMax_(-1.)
  , debug_(cfg.getUntrackedParameter<bool>("debug", false))
{
  std::string signalConeSizeFormulaName = Form("signalConeSizeFormula%i", signalConeSizeFormula_instance_counter_);
  ++signalConeSizeFormula_instance_counter_;
  std::string signalConeSizeFormula_original = cfg.getParameter<std::string>("signalConeSize");
  TString signalConeSizeFormula_modified = signalConeSizeFormula_original.data();
  signalConeSizeFormula_modified.ReplaceAll("pt", "x");
  signalConeSizeFormula_ = new TFormula(signalConeSizeFormulaName.data(), signalConeSizeFormula_modified.Data(), false, false);
  if ( !(signalConeSizeFormula_->Compile() == 0) )
  {
    throw cms::Exception("L1HPSPFTauBuilder")
      << "Invalid Configuration parameter 'signalConeSize' = '" << signalConeSizeFormula_original << "' !!\n";
  }
  assert(max_signalConeSize_ >= min_signalConeSize_);

  isolationConeSize2_ = isolationConeSize_*isolationConeSize_;

  if ( debug_ )
  {
    std::cout << "setting Quality cuts for signal PFCands:" << std::endl;
  }
  edm::ParameterSet cfg_signalQualityCuts = cfg.getParameter<edm::ParameterSet>("signalQualityCuts");
  signalQualityCuts_dzCut_disabled_           = readL1PFTauQualityCuts(cfg_signalQualityCuts,    "disabled",        debug_);
  signalQualityCuts_dzCut_enabled_primary_    = readL1PFTauQualityCuts(cfg_signalQualityCuts,    "enabled_primary", debug_);
  if ( debug_ )
  {
    std::cout << "setting Quality cuts for isolation PFCands:" << std::endl;
  }
  edm::ParameterSet cfg_isolationQualityCuts = cfg.getParameter<edm::ParameterSet>("isolationQualityCuts");
  isolationQualityCuts_dzCut_disabled_        = readL1PFTauQualityCuts(cfg_isolationQualityCuts, "disabled",        debug_);
  isolationQualityCuts_dzCut_enabled_primary_ = readL1PFTauQualityCuts(cfg_isolationQualityCuts, "enabled_primary", debug_);
  isolationQualityCuts_dzCut_enabled_pileup_  = readL1PFTauQualityCuts(cfg_isolationQualityCuts, "enabled_pileup",  debug_);

  if ( inputFileName_rhoCorr_ != "" && histogramName_rhoCorr_ != "" ) 
  {
    LocalFileInPath inputFileName_rhoCorr_fip(inputFileName_rhoCorr_);
    inputFile_rhoCorr_ = openFile(inputFileName_rhoCorr_fip);
    TH1* histogram_rhoCorr_temp = loadTH1(inputFile_rhoCorr_, histogramName_rhoCorr_);
    std::string histogramName_rhoCorr = Form("%s_cloned", histogram_rhoCorr_->GetName());
    histogram_rhoCorr_ = (TH1*)histogram_rhoCorr_temp->Clone(histogramName_rhoCorr.data());
    int numBins = histogram_rhoCorr_->GetNbinsX();
    for ( int idxBin = 1; idxBin <= numBins; ++idxBin )
    {
      double binContent = histogram_rhoCorr_->GetBinContent(idxBin);
      if ( binContent > histogram_rhoCorr_yMax_ ) 
      {
	histogram_rhoCorr_yMax_ = binContent;
      }
    }
    delete inputFile_rhoCorr_;
    inputFile_rhoCorr_ = nullptr;
  }
}

L1HPSPFTauBuilder::~L1HPSPFTauBuilder()
{
  delete histogram_rhoCorr_;
}

void L1HPSPFTauBuilder::reset()
{
  signalConeSize_ = 0.;
  signalConeSize2_ = 0.;

  l1PFCandProductID_ = edm::ProductID();
  isPFCandSeeded_ = false;
  l1PFCand_seed_ = l1t::PFCandidateRef();
  isPFJetSeeded_ = false;
  l1PFJet_seed_ = l1t::PFJetRef();
  l1PFTauSeed_eta_ = 0.;
  l1PFTauSeed_phi_ = 0.;
  l1PFTauSeed_zVtx_ = 0.;
  sumAllL1PFCandidates_pt_ = 0.;
  primaryVertex_ = l1t::VertexRef();
  l1PFTau_ = l1t::L1HPSPFTau();
  rho_ = 0.;

  strip_p4_ = reco::Particle::LorentzVector(0.,0.,0.,0.);

  signalAllL1PFCandidates_.clear();
  signalChargedHadrons_.clear();
  signalElectrons_.clear();
  signalNeutralHadrons_.clear();
  signalPhotons_.clear();
  signalMuons_.clear();
  
  stripAllL1PFCandidates_.clear();
  stripElectrons_.clear();
  stripPhotons_.clear();

  isoAllL1PFCandidates_.clear();
  isoChargedHadrons_.clear();
  isoElectrons_.clear();
  isoNeutralHadrons_.clear();
  isoPhotons_.clear();
  isoMuons_.clear();
  
  sumAllL1PFCandidates_.clear();
  sumChargedHadrons_.clear();
  sumElectrons_.clear();
  sumNeutralHadrons_.clear();
  sumPhotons_.clear();
  sumMuons_.clear();

  sumChargedIsoPileup_ = 0.;
  rhoCorr_ = 0.;
}

void L1HPSPFTauBuilder::setL1PFCandProductID(const edm::ProductID& l1PFCandProductID)
{
  l1PFCandProductID_ = l1PFCandProductID;
}

void L1HPSPFTauBuilder::setVertex(const l1t::VertexRef& primaryVertex)
{
  primaryVertex_ = primaryVertex;
}
 
void L1HPSPFTauBuilder::setL1PFTauSeed(const l1t::PFCandidateRef& l1PFCand_seed)
{
  if ( debug_ )
  {
    std::cout << "<L1HPSPFTauBuilder::setL1PFTauSeed>:" << std::endl;
    std::cout << "seeding L1HPSPFTau with ChargedPFCand:";
    printPFCand(std::cout, *l1PFCand_seed, primaryVertex_);
  }

  l1PFCand_seed_ = l1PFCand_seed;
  l1PFTauSeed_eta_ = l1PFCand_seed->eta();
  l1PFTauSeed_phi_ = l1PFCand_seed->phi();
  if ( l1PFCand_seed->charge() != 0 && l1PFCand_seed->pfTrack().isNonnull() )
  {
    l1PFTauSeed_zVtx_ = l1PFCand_seed->pfTrack()->vertex().z();
    isPFCandSeeded_ = true;
  }
}
 
void L1HPSPFTauBuilder::setL1PFTauSeed(const l1t::PFJetRef& l1PFJet_seed)
{
  if ( debug_ )
  {
    std::cout << "<L1HPSPFTauBuilder::setL1PFTauSeed>:" << std::endl;
    std::cout << "seeding L1HPSPFTau with PFJet:";
    std::cout << " pT = " << l1PFJet_seed->pt()  << ", eta = " << l1PFJet_seed->eta() << ", phi = " << l1PFJet_seed->phi() << std::endl;
  }

  l1PFJet_seed_ = l1PFJet_seed;
  reco::Candidate::LorentzVector l1PFTauSeed_p4;
  float l1PFTauSeed_zVtx = 0.;
  bool l1PFTauSeed_hasVtx = false;
  float max_chargedPFCand_pt = -1.;
  size_t numConstituents = l1PFJet_seed->numberOfDaughters();
  for ( size_t idxConstituent = 0; idxConstituent < numConstituents; ++idxConstituent )
  {
    const l1t::PFCandidate* l1PFCand = dynamic_cast<const l1t::PFCandidate*>(l1PFJet_seed->daughter(idxConstituent));
    if ( !l1PFCand )
    {
      throw cms::Exception("L1HPSPFTauBuilder")
	<< "PFJet was not built from l1t::PFCandidates !!\n";
    }
    if ( l1PFCand->id() == l1t::PFCandidate::ChargedHadron ||
	 l1PFCand->id() == l1t::PFCandidate::Electron      ||
	 l1PFCand->id() == l1t::PFCandidate::Photon        ||
	 l1PFCand->id() == l1t::PFCandidate::Muon          ) 
    {
      l1PFTauSeed_p4 += l1PFCand->p4();
      if ( l1PFCand->charge() != 0 && l1PFCand->pfTrack().isNonnull() && l1PFCand->pt() > max_chargedPFCand_pt )
      {
	l1PFTauSeed_zVtx = l1PFCand->pfTrack()->vertex().z();
	l1PFTauSeed_hasVtx = true;
	max_chargedPFCand_pt = l1PFCand->pt();
      }
    }  
  }
  if ( l1PFTauSeed_p4.pt() > 1. && l1PFTauSeed_hasVtx )
  {
    l1PFTauSeed_eta_  = l1PFTauSeed_p4.eta();
    l1PFTauSeed_phi_  = l1PFTauSeed_p4.phi();
    l1PFTauSeed_zVtx_ = l1PFTauSeed_zVtx; 
    isPFJetSeeded_ = true;
  }
}

void L1HPSPFTauBuilder::addL1PFCandidates(const std::vector<l1t::PFCandidateRef>& l1PFCands)
{
  if ( debug_ )
  {
    std::cout << "<L1HPSPFTauBuilder::addL1PFCandidates>:" << std::endl;
  }

  // do not build tau candidates for which no reference z-position exists,
  // as in this case charged PFCands originating from the primary (hard-scatter) interaction 
  // cannot be distinguished from charged PFCands originating from pileup
  if ( !(isPFCandSeeded_ || isPFJetSeeded_) ) return; 

  for ( auto l1PFCand : l1PFCands ) 
  {
    if ( !isWithinIsolationCone(*l1PFCand) )
      continue;
    sumAllL1PFCandidates_.push_back(l1PFCand);
    if ( l1PFCand->id() == l1t::PFCandidate::ChargedHadron ) 
    {
      sumChargedHadrons_.push_back(l1PFCand);
    }
    else if ( l1PFCand->id() == l1t::PFCandidate::Electron ) 
    {
      sumElectrons_.push_back(l1PFCand);
    }
    else if ( l1PFCand->id() == l1t::PFCandidate::NeutralHadron ) 
    {
      sumNeutralHadrons_.push_back(l1PFCand);
    }
    else if ( l1PFCand->id() == l1t::PFCandidate::Photon ) 
    {
      sumPhotons_.push_back(l1PFCand);
    }
    else if ( l1PFCand->id() == l1t::PFCandidate::Muon ) 
    {
      sumMuons_.push_back(l1PFCand);
    }
  }

  for ( auto l1PFCand : sumAllL1PFCandidates_ )
  {
    sumAllL1PFCandidates_pt_ += l1PFCand->pt();
  }
  signalConeSize_ = signalConeSizeFormula_->Eval(sumAllL1PFCandidates_pt_);
  if ( signalConeSize_ < min_signalConeSize_ ) signalConeSize_ = min_signalConeSize_;
  if ( signalConeSize_ > max_signalConeSize_ ) signalConeSize_ = max_signalConeSize_;
  signalConeSize2_ = signalConeSize_*signalConeSize_;

  for ( auto l1PFCand : sumAllL1PFCandidates_ ) 
  {
    if ( debug_ ) 
    {
      printPFCand(std::cout, *l1PFCand, primaryVertex_);
    }

    bool isSignalPFCand = false;
    bool isStripPFCand = false;
    bool isElectron_or_Photon = l1PFCand->id() == l1t::PFCandidate::Electron || l1PFCand->id() == l1t::PFCandidate::Photon;
    bool isChargedHadron = l1PFCand->id() == l1t::PFCandidate::ChargedHadron;
    if ( isWithinSignalCone(*l1PFCand) && !(isChargedHadron && signalChargedHadrons_.size() > 3) ) 
    { 
      isSignalPFCand = true;
    }
    if ( isElectron_or_Photon && isWithinStrip(*l1PFCand) ) 
    { 
      if ( useStrips_ ) 
      {
	isSignalPFCand = true;
      }
      isStripPFCand = true;
    }
    bool passesSignalQualityCuts = isSelected(signalQualityCuts_dzCut_enabled_primary_, *l1PFCand, l1PFTauSeed_zVtx_);
    if ( isSignalPFCand && passesSignalQualityCuts )
    {
      signalAllL1PFCandidates_.push_back(l1PFCand);  
      if ( l1PFCand->id() == l1t::PFCandidate::ChargedHadron ) 
      {
	signalChargedHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand->id() == l1t::PFCandidate::Electron ) 
      {
	signalElectrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand->id() == l1t::PFCandidate::NeutralHadron ) 
      {
	signalNeutralHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand->id() == l1t::PFCandidate::Photon ) 
      {
	signalPhotons_.push_back(l1PFCand);
      }
      else if ( l1PFCand->id() == l1t::PFCandidate::Muon ) 
      {
	signalMuons_.push_back(l1PFCand);
      }
    } 
    if ( isStripPFCand && passesSignalQualityCuts )
    {
      stripAllL1PFCandidates_.push_back(l1PFCand);  
      if ( l1PFCand->id() == l1t::PFCandidate::Electron ) 
      {
	stripElectrons_.push_back(l1PFCand);
	strip_p4_ += l1PFCand->p4();
      }
      else if ( l1PFCand->id() == l1t::PFCandidate::Photon ) 
      {
	stripPhotons_.push_back(l1PFCand);
	strip_p4_ += l1PFCand->p4();
      }
      else assert(0);
    }
    
    bool isIsolationPFCand = isWithinIsolationCone(*l1PFCand) && !isSignalPFCand;
    bool passesIsolationQualityCuts = isSelected(isolationQualityCuts_dzCut_enabled_primary_, *l1PFCand, l1PFTauSeed_zVtx_);
    if ( isIsolationPFCand && passesIsolationQualityCuts )
    {
      isoAllL1PFCandidates_.push_back(l1PFCand);
      if ( l1PFCand->id() == l1t::PFCandidate::ChargedHadron ) 
      {
	isoChargedHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand->id() == l1t::PFCandidate::Electron )
      {
	isoElectrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand->id() == l1t::PFCandidate::NeutralHadron ) 
      {
	isoNeutralHadrons_.push_back(l1PFCand);
      }
      else if ( l1PFCand->id() == l1t::PFCandidate::Photon )
      {
	isoPhotons_.push_back(l1PFCand);
      }
      else if ( l1PFCand->id() == l1t::PFCandidate::Muon )
      {
	isoMuons_.push_back(l1PFCand);
      }
    }

    if ( debug_ )
    {
      std::cout << "dR = " << reco::deltaR(l1PFCand->eta(), l1PFCand->phi(), l1PFTauSeed_eta_, l1PFTauSeed_phi_) << ":"
		<< " isSignalPFCand = " << isSignalPFCand << ", isStripPFCand = " << isStripPFCand << " (passesSignalQualityCuts = " << passesSignalQualityCuts << "),"
		<< " isIsolationPFCand = " << isIsolationPFCand << " (passesIsolationQualityCuts = " << passesIsolationQualityCuts << ")" << std::endl;
    }
  }

  
  for ( auto l1PFCand : l1PFCands ) 
  {
    if( !isWithinIsolationCone(*l1PFCand) )
      continue;

    if ( l1PFCand->charge() != 0 && isSelected(isolationQualityCuts_dzCut_enabled_pileup_, *l1PFCand, l1PFTauSeed_zVtx_) )
    {
      sumChargedIsoPileup_ += l1PFCand->pt();
    }
  }
}

void L1HPSPFTauBuilder::setRho(double rho)
{
  rho_ = rho;
}

bool L1HPSPFTauBuilder::isWithinSignalCone(const l1t::PFCandidate& l1PFCand)
{
  if ( isPFCandSeeded_ || isPFJetSeeded_ )
  {
    double deltaEta = l1PFCand.eta() - l1PFTauSeed_eta_;
    double deltaPhi = l1PFCand.phi() - l1PFTauSeed_phi_;
    if ( (deltaEta*deltaEta + deltaPhi*deltaPhi) < signalConeSize2_ ) return true;
  }
  return false;
}

bool L1HPSPFTauBuilder::isWithinStrip(const l1t::PFCandidate& l1PFCand)
{
  if ( isPFCandSeeded_ || isPFJetSeeded_ )
  {
    double deltaEta = l1PFCand.eta() - l1PFTauSeed_eta_;
    double deltaPhi = l1PFCand.phi() - l1PFTauSeed_phi_;
    if ( std::fabs(deltaEta) < stripSize_eta_ && std::fabs(deltaPhi) < stripSize_phi_ ) return true;
  }
  return false;
}

bool L1HPSPFTauBuilder::isWithinIsolationCone(const l1t::PFCandidate& l1PFCand)
{
  double deltaEta = l1PFCand.eta() - l1PFTauSeed_eta_;
  double deltaPhi = l1PFCand.phi() - l1PFTauSeed_phi_;
  if ( (deltaEta*deltaEta + deltaPhi*deltaPhi) < isolationConeSize2_ ) return true;
  else return false;
}

void L1HPSPFTauBuilder::buildL1PFTau()
{
  reco::Particle::LorentzVector l1PFTau_p4;
  for ( auto l1PFCand : signalAllL1PFCandidates_ )
  {
    if ( l1PFCand->id() == l1t::PFCandidate::ChargedHadron ||
	 l1PFCand->id() == l1t::PFCandidate::Electron      ||
	 l1PFCand->id() == l1t::PFCandidate::Photon        )
    {
      l1PFTau_p4 += l1PFCand->p4();
      if ( l1PFCand->charge() != 0 && (l1PFTau_.leadChargedPFCand_.isNull() || l1PFCand->pt() > l1PFTau_.leadChargedPFCand_->pt()) ) 
      {
        l1PFTau_.leadChargedPFCand_ = l1PFCand;
      }
    }
  }
  l1PFTau_.setP4(l1PFTau_p4);

  l1PFTau_.seedChargedPFCand_ = l1PFCand_seed_;
  l1PFTau_.seedPFJet_ = l1PFJet_seed_;

  l1PFTau_.signalAllL1PFCandidates_ = convertToRefVector(signalAllL1PFCandidates_);
  l1PFTau_.signalChargedHadrons_ = convertToRefVector(signalChargedHadrons_);
  l1PFTau_.signalElectrons_ = convertToRefVector(signalElectrons_);
  l1PFTau_.signalNeutralHadrons_ = convertToRefVector(signalNeutralHadrons_);
  l1PFTau_.signalPhotons_ = convertToRefVector(signalPhotons_);
  l1PFTau_.signalMuons_ = convertToRefVector(signalMuons_);
  
  l1PFTau_.stripAllL1PFCandidates_ = convertToRefVector(stripAllL1PFCandidates_);
  l1PFTau_.stripElectrons_ = convertToRefVector(stripElectrons_);
  l1PFTau_.stripPhotons_ = convertToRefVector(stripPhotons_);

  l1PFTau_.isoAllL1PFCandidates_ = convertToRefVector(isoAllL1PFCandidates_);
  l1PFTau_.isoChargedHadrons_ = convertToRefVector(isoChargedHadrons_);
  l1PFTau_.isoElectrons_ = convertToRefVector(isoElectrons_);
  l1PFTau_.isoNeutralHadrons_ = convertToRefVector(isoNeutralHadrons_);
  l1PFTau_.isoPhotons_ = convertToRefVector(isoPhotons_);
  l1PFTau_.isoMuons_ = convertToRefVector(isoMuons_);
  
  l1PFTau_.sumAllL1PFCandidates_ = convertToRefVector(sumAllL1PFCandidates_);
  l1PFTau_.sumChargedHadrons_ = convertToRefVector(sumChargedHadrons_);
  l1PFTau_.sumElectrons_ = convertToRefVector(sumElectrons_);
  l1PFTau_.sumNeutralHadrons_ = convertToRefVector(sumNeutralHadrons_);
  l1PFTau_.sumPhotons_ = convertToRefVector(sumPhotons_);
  l1PFTau_.sumMuons_ = convertToRefVector(sumMuons_);

  l1PFTau_.primaryVertex_ = primaryVertex_;

  if ( l1PFTau_.signalChargedHadrons_.size() > 1 ) 
  { 
    if ( strip_p4_.pt() < 5.  ) l1PFTau_.tauType_ = l1t::L1HPSPFTau::kThreeProng0Pi0;
    else                        l1PFTau_.tauType_ = l1t::L1HPSPFTau::kThreeProng1Pi0;
  } 
  else 
  {
    if ( strip_p4_.pt() < 5.  ) l1PFTau_.tauType_ = l1t::L1HPSPFTau::kOneProng0Pi0;
    else                        l1PFTau_.tauType_ = l1t::L1HPSPFTau::kOneProng1Pi0;
  }

  l1PFTau_.strip_p4_ = strip_p4_;

  l1PFTau_.sumAllL1PFCandidates_pt_ = sumAllL1PFCandidates_pt_;
  l1PFTau_.signalConeSize_ = signalConeSize_;
  l1PFTau_.isolationConeSize_ = isolationConeSize_;

  double sumChargedIso = 0.;
  double sumNeutralIso = 0.;
  for ( auto l1PFCand : isoAllL1PFCandidates_ )
  {
    if ( l1PFCand->charge() != 0 ) 
    { 
      sumChargedIso += l1PFCand->pt();
    }
    else if ( l1PFCand->id() == l1t::PFCandidate::Photon ) 
    { 
      sumNeutralIso += l1PFCand->pt();
    }
  }
  l1PFTau_.sumChargedIso_ = sumChargedIso;
  l1PFTau_.sumNeutralIso_ = sumNeutralIso;
  const double weightNeutralIso = 1.;
  const double offsetNeutralIso = 0.;
  l1PFTau_.sumCombinedIso_ = sumChargedIso + weightNeutralIso*(sumNeutralIso - offsetNeutralIso);
  l1PFTau_.sumChargedIsoPileup_ = sumChargedIsoPileup_;

  if ( histogram_rhoCorr_ && histogram_rhoCorr_yMax_ > 0. ) 
  {
    const double isolationConeArea = TMath::Pi()*0.4*0.4;
    rhoCorr_ = isolationConeArea*rho_;
    int idxBin = histogram_rhoCorr_->FindBin(std::fabs(l1PFTau_.eta()));
    if ( idxBin >= 1 && idxBin <= histogram_rhoCorr_->GetNbinsX() )
    {
      rhoCorr_ *= histogram_rhoCorr_->GetBinContent(idxBin)/histogram_rhoCorr_yMax_;
      l1PFTau_.rhoCorr_ = rhoCorr_;
    } 
    else
    {
      std::cerr << "Warning in <L1HPSPFTauBuilder>: Failed to compute rho correction for abs(eta) = " << l1PFTau_.eta() << " !!" << std::endl;
    }
  }

  if ( l1PFTau_.sumChargedIso() < 20.0 ) 
  {
    l1PFTau_.passVLooseIso_ = true;
  }
  if ( l1PFTau_.sumChargedIso() < 10.0 ) 
  {
    l1PFTau_.passLooseIso_ = true;
  }
  if ( l1PFTau_.sumChargedIso() <  5.0 ) 
  {
    l1PFTau_.passMediumIso_ = true;
  }
  if ( l1PFTau_.sumChargedIso() <  2.5 ) 
  {
    l1PFTau_.passTightIso_ = true;
  }

  if ( l1PFTau_.sumChargedIso() < 0.40*l1PFTau_.pt() ) 
  {
    l1PFTau_.passVLooseRelIso_ = true;
  }
  if ( l1PFTau_.sumChargedIso() < 0.20*l1PFTau_.pt() ) 
  {
    l1PFTau_.passLooseRelIso_ = true;
  }
  if ( l1PFTau_.sumChargedIso() < 0.10*l1PFTau_.pt() ) 
  {
    l1PFTau_.passMediumRelIso_ = true;
  }
  if ( l1PFTau_.sumChargedIso() < 0.05*l1PFTau_.pt() ) 
  {
    l1PFTau_.passTightRelIso_ = true;
  }
}

l1t::PFCandidateRefVector L1HPSPFTauBuilder::convertToRefVector(const std::vector<l1t::PFCandidateRef>& l1PFCands)
{
  l1t::PFCandidateRefVector l1PFCands_refVector(l1PFCandProductID_);
  for ( auto l1PFCand : l1PFCands ) 
  {
    l1PFCands_refVector.push_back(l1PFCand);
  }
  return l1PFCands_refVector;
}
