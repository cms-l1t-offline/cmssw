#include "L1Trigger/Phase2L1Taus/interface/L1HPSPFTauProducer.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include <cmath> // std::fabs

L1HPSPFTauProducer::L1HPSPFTauProducer(const edm::ParameterSet& cfg) 
  : moduleLabel_(cfg.getParameter<std::string>("@module_label"))
  , tauBuilder_(nullptr)
  , useChargedPFCandSeeds_(cfg.getParameter<bool>("useChargedPFCandSeeds"))
  , min_seedChargedPFCand_pt_(cfg.getParameter<double>("min_seedChargedPFCand_pt"))
  , max_seedChargedPFCand_eta_(cfg.getParameter<double>("max_seedChargedPFCand_eta"))
  , max_seedChargedPFCand_dz_(cfg.getParameter<double>("max_seedChargedPFCand_dz"))
  , usePFJetSeeds_(cfg.getParameter<bool>("usePFJetSeeds"))
  , min_seedPFJet_pt_(cfg.getParameter<double>("min_seedPFJet_pt"))
  , max_seedPFJet_eta_(cfg.getParameter<double>("max_seedPFJet_eta"))
  , min_PFTau_pt_(cfg.getParameter<double>("min_PFTau_pt"))
  , max_PFTau_eta_(cfg.getParameter<double>("max_PFTau_eta"))
  , min_leadChargedPFCand_pt_(cfg.getParameter<double>("min_leadChargedPFCand_pt"))
  , max_leadChargedPFCand_eta_(cfg.getParameter<double>("max_leadChargedPFCand_eta"))
  , max_leadChargedPFCand_dz_(cfg.getParameter<double>("max_leadChargedPFCand_dz"))
  , max_chargedIso_(cfg.getParameter<double>("max_chargedIso"))
  , max_chargedRelIso_(cfg.getParameter<double>("max_chargedRelIso"))
  , deltaR_cleaning_(cfg.getParameter<double>("deltaR_cleaning"))
  , applyPreselection_(cfg.getParameter<bool>("applyPreselection"))
  , debug_(cfg.getUntrackedParameter<bool>("debug", false))
{
  if ( debug_ )
  {
    std::cout << "<L1HPSPFTauProducer::L1HPSPFTauProducer (moduleLabel = " << moduleLabel_ << ")>:" << std::endl;
  }

  tauBuilder_ = new L1HPSPFTauBuilder(cfg);

  srcL1PFCands_ = cfg.getParameter<edm::InputTag>("srcL1PFCands");
  tokenL1PFCands_ = consumes<l1t::PFCandidateCollection>(srcL1PFCands_);
  srcL1PFJets_ = cfg.getParameter<edm::InputTag>("srcL1PFJets");
  if ( usePFJetSeeds_ )
  {
    tokenL1PFJets_ = consumes<l1t::PFJetCollection>(srcL1PFJets_);
  }
  srcL1Vertices_ = cfg.getParameter<edm::InputTag>("srcL1Vertices");
  if ( srcL1Vertices_.label() != "" ) 
  {
    tokenL1Vertices_ = consumes<l1t::VertexCollection>(srcL1Vertices_);
  }
  srcRho_ = cfg.getParameter<edm::InputTag>("srcRho");
  if ( srcRho_.label() != "" ) 
  {
    tokenRho_ = consumes<double>(srcRho_);
  }

  deltaR2_cleaning_ = deltaR_cleaning_*deltaR_cleaning_;

  edm::ParameterSet cfg_signalQualityCuts = cfg.getParameter<edm::ParameterSet>("signalQualityCuts");
  signalQualityCuts_dzCut_disabled_ = readL1PFTauQualityCuts(cfg_signalQualityCuts, "disabled");
  edm::ParameterSet cfg_isolationQualityCuts = cfg.getParameter<edm::ParameterSet>("isolationQualityCuts");
  isolationQualityCuts_dzCut_disabled_ = readL1PFTauQualityCuts(cfg_isolationQualityCuts, "disabled");

  produces<l1t::L1HPSPFTauCollection>();
}

L1HPSPFTauProducer::~L1HPSPFTauProducer()
{
  delete tauBuilder_;
}

namespace
{
  bool
  isHigherPt_pfCandRef(const l1t::PFCandidateRef& l1PFCand1,
		       const l1t::PFCandidateRef& l1PFCand2)
  {
    return l1PFCand1->pt() > l1PFCand2->pt();
  }

  bool
  isHigherPt_pfTau(const l1t::L1HPSPFTau& l1PFTau1,
		   const l1t::L1HPSPFTau& l1PFTau2)
  {
    return l1PFTau1.pt() > l1PFTau2.pt();
  }
}

void L1HPSPFTauProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  std::unique_ptr<l1t::L1HPSPFTauCollection> l1PFTauCollection_cleaned(new l1t::L1HPSPFTauCollection());

  edm::Handle<l1t::PFCandidateCollection> l1PFCands;
  evt.getByToken(tokenL1PFCands_, l1PFCands);

  l1t::VertexRef primaryVertex;
  float primaryVertex_z = 0.;
  if ( srcL1Vertices_.label() != "" ) 
  {
    edm::Handle<l1t::VertexCollection> vertices;
    evt.getByToken(tokenL1Vertices_, vertices);
    if ( vertices->size() > 0 ) 
    {
      primaryVertex = l1t::VertexRef(vertices, 0);
      primaryVertex_z = primaryVertex->z0();
    }
  }

  edm::Handle<double> rho;
  if ( srcRho_.label() != "" ) 
  {
    evt.getByToken(tokenRho_, rho);
  }

  if ( debug_ ) 
  {
    std::cout << "BEFORE selection:" << std::endl;
    for ( auto l1PFCand : *l1PFCands )
    {
      printPFCand(std::cout, l1PFCand, primaryVertex_z);
    }
  }

  // build collection of selected PFCandidates
  std::vector<l1t::PFCandidateRef> selectedL1PFCands_signalQualityCuts;
  std::vector<l1t::PFCandidateRef> selectedL1PFCands_signal_or_isolationQualityCuts;
  size_t numL1PFCands = l1PFCands->size();
  for ( size_t idxL1PFCand = 0; idxL1PFCand < numL1PFCands; ++idxL1PFCand ) {
    l1t::PFCandidateRef l1PFCand(l1PFCands, idxL1PFCand);
    bool passesSignalQualityCuts    = isSelected(signalQualityCuts_dzCut_disabled_,    *l1PFCand, primaryVertex_z);
    bool passesIsolationQualityCuts = isSelected(isolationQualityCuts_dzCut_disabled_, *l1PFCand, primaryVertex_z);
    if ( passesSignalQualityCuts )
    {
      selectedL1PFCands_signalQualityCuts.push_back(l1PFCand);
    }
    if ( passesSignalQualityCuts || passesIsolationQualityCuts )
    {
      selectedL1PFCands_signal_or_isolationQualityCuts.push_back(l1PFCand);
    }
  }

  // sort PFCandidate collection by decreasing pT
  std::sort(selectedL1PFCands_signalQualityCuts.begin(), selectedL1PFCands_signalQualityCuts.end(), isHigherPt_pfCandRef);
  std::sort(selectedL1PFCands_signal_or_isolationQualityCuts.begin(), selectedL1PFCands_signal_or_isolationQualityCuts.end(), isHigherPt_pfCandRef);

  if ( debug_ ) 
  {
    std::cout << "AFTER selection (signalQualityCuts):" << std::endl;
    for ( auto l1PFCand : selectedL1PFCands_signalQualityCuts )
    {
      printPFCand(std::cout, *l1PFCand, primaryVertex_z);
    }
  }

  l1t::L1HPSPFTauCollection l1PFTauCollection_uncleaned;

  if ( useChargedPFCandSeeds_ ) 
  {
    for ( auto l1PFCand : selectedL1PFCands_signalQualityCuts ) 
    {
      if ( l1PFCand->charge() != 0 && l1PFCand->pt() > min_seedChargedPFCand_pt_ && std::fabs(l1PFCand->eta()) < max_seedChargedPFCand_eta_ )
      {
        bool isFromPrimaryVertex = false;
        if ( primaryVertex.get() ) 
        {
          l1t::PFTrackRef l1PFTrack = l1PFCand->pfTrack();
          double dz = std::fabs(l1PFTrack->vertex().z() - primaryVertex_z);
          if ( dz < max_seedChargedPFCand_dz_ ) 
          {
  	    isFromPrimaryVertex = true;
          }
        } 
        else 
        {
          isFromPrimaryVertex = true;
        }
        if ( isFromPrimaryVertex ) 
        {
	  tauBuilder_->reset();
          tauBuilder_->setL1PFCandProductID(l1PFCands.id());
	  tauBuilder_->setVertex(primaryVertex);
	  tauBuilder_->setL1PFTauSeed(l1PFCand);
	  tauBuilder_->addL1PFCandidates(selectedL1PFCands_signal_or_isolationQualityCuts);
	  if ( srcRho_.label() != "" ) 
	  {
	    tauBuilder_->setRho(*rho);
	  }
	  tauBuilder_->buildL1PFTau();
	  l1t::L1HPSPFTau l1PFTau = tauBuilder_->getL1PFTau();
	  if ( l1PFTau.pt() > 1. ) l1PFTauCollection_uncleaned.push_back(l1PFTau);
        }
      }
    }
  }

  if ( usePFJetSeeds_ )
  {
    edm::Handle<l1t::PFJetCollection> l1PFJets;
    evt.getByToken(tokenL1PFJets_, l1PFJets);

    size_t numL1PFJets = l1PFJets->size();    
    for ( size_t idxL1PFJet = 0; idxL1PFJet < numL1PFJets; ++idxL1PFJet ) {
      l1t::PFJetRef l1PFJet(l1PFJets, idxL1PFJet);
      if ( l1PFJet->pt() > min_seedPFJet_pt_ && std::fabs(l1PFJet->eta()) < max_seedPFJet_eta_ )
      {
	tauBuilder_->reset();
	tauBuilder_->setL1PFCandProductID(l1PFCands.id());
	tauBuilder_->setVertex(primaryVertex);
	tauBuilder_->setL1PFTauSeed(l1PFJet);
	tauBuilder_->addL1PFCandidates(selectedL1PFCands_signal_or_isolationQualityCuts);
	if ( srcRho_.label() != "" ) 
	{
	  tauBuilder_->setRho(*rho);
	}
	tauBuilder_->buildL1PFTau();
	l1t::L1HPSPFTau l1PFTau = tauBuilder_->getL1PFTau();
	if ( l1PFTau.pt() > 1. ) l1PFTauCollection_uncleaned.push_back(l1PFTau);
      }
    }
  }

  // sort PFTau candidate collection by decreasing pT
  std::sort(l1PFTauCollection_uncleaned.begin(), l1PFTauCollection_uncleaned.end(), isHigherPt_pfTau);

  if ( debug_ ) 
  {
    std::cout << "BEFORE cleaning:" << std::endl;
    for ( size_t idx = 0; idx < l1PFTauCollection_uncleaned.size(); ++idx )
    {
      const l1t::L1HPSPFTau& l1PFTau = l1PFTauCollection_uncleaned.at(idx);
      std::cout << "L1HPSPFTau #" << idx << ": " << l1PFTau;
    }
  }

  for ( auto l1PFTau : l1PFTauCollection_uncleaned )
  {
    if (  applyPreselection_ && 
         !(l1PFTau.pt() > min_PFTau_pt_ && std::fabs(l1PFTau.eta()) < max_PFTau_eta_ &&
           l1PFTau.leadChargedPFCand().isNonnull() && 
	   l1PFTau.leadChargedPFCand()->pt() > min_leadChargedPFCand_pt_ && std::fabs(l1PFTau.leadChargedPFCand()->eta()) < max_leadChargedPFCand_eta_ && 
           (srcL1Vertices_.label() == "" || 
	    (primaryVertex.isNonnull() && l1PFTau.leadChargedPFCand()->pfTrack().isNonnull() &&
	     std::fabs(l1PFTau.leadChargedPFCand()->pfTrack()->vertex().z() - primaryVertex->z0()) < max_leadChargedPFCand_dz_)) &&
           l1PFTau.sumChargedIso() < max_chargedIso_ && l1PFTau.sumChargedIso() < max_chargedRelIso_*l1PFTau.pt()) ) continue;

    bool isOverlap = false;
    for ( auto l1PFTau2 : *l1PFTauCollection_cleaned )
    {
      double deltaEta = l1PFTau.eta() - l1PFTau2.eta();
      double deltaPhi = l1PFTau.phi() - l1PFTau2.phi();
      if ( (deltaEta*deltaEta + deltaPhi*deltaPhi) < deltaR2_cleaning_ ) 
      {
	isOverlap = true;
      }      
    }
    if ( !isOverlap )
    {
      l1PFTauCollection_cleaned->push_back(l1PFTau);
    }
  }
  
  if ( debug_ ) 
  {
    std::cout << "AFTER cleaning:" << std::endl;
    for ( size_t idx = 0; idx < l1PFTauCollection_cleaned->size(); ++idx )
    {
      const l1t::L1HPSPFTau& l1PFTau = l1PFTauCollection_cleaned->at(idx);
      std::cout << "L1HPSPFTau #" << idx << ": " << l1PFTau;
    }
  }

  evt.put(std::move(l1PFTauCollection_cleaned));
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1HPSPFTauProducer);
