// -*- C++ -*-
//
// Package:    L1TriggerDPG/L1Ntuples
// Class:      L1CustomGenTreeProducer
// 
/**\class L1CustomGenTreeProducer L1CustomGenTreeProducer.cc L1TriggerDPG/L1Ntuples/src/L1CustomGenTreeProducer.cc

Description: Produce L1 Extra tree

Implementation:
     
*/
//
// Original Author:  
//         Created:  
// $Id: L1CustomGenTreeProducer.cc,v 1.8 2012/08/29 12:44:03 jbrooke Exp $
//
//


// system include files
#include <memory>
#include <string>

// framework
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// input data formats
#include "DataFormats/JetReco/interface/GenJetCollection.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"


// ROOT output stuff
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include "L1Trigger/L1TNtuples/interface/L1AnalysisCustomGeneratorDataFormat.h"

// Namespaces                                                                                                                                                  
using namespace std;
using namespace edm;


//
// class declaration
//

class L1CustomGenTreeProducer : public edm::EDAnalyzer {
public:
  explicit L1CustomGenTreeProducer(const edm::ParameterSet&);
  ~L1CustomGenTreeProducer() override;
  
  
private:
  void beginJob(void) override ;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;  
  template<typename T> void print(T t, const int& width=12, const int& precision=3);
  virtual void PrintGenParticle(reco::GenParticle genP, int genP_index, string title="GenParticles", bool printHeader=true);
  
private:

  unsigned maxL1Upgrade_;

  // output file
  edm::Service<TFileService> fs_;
  
  // tree
  TTree * tree_;
 
  // data format
  std::unique_ptr<L1Analysis::L1AnalysisCustomGeneratorDataFormat>       l1GenData_;

  // EDM input tags
  edm::EDGetTokenT<reco::GenJetCollection> genJetToken_;
  edm::EDGetTokenT<reco::GenParticleCollection> genParticleToken_;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo> > pileupInfoToken_;

};



L1CustomGenTreeProducer::L1CustomGenTreeProducer(const edm::ParameterSet& iConfig)
{

  genJetToken_ = consumes<reco::GenJetCollection>(iConfig.getUntrackedParameter<edm::InputTag>("genJetToken"));
  genParticleToken_ = consumes<reco::GenParticleCollection>(iConfig.getUntrackedParameter<edm::InputTag>("genParticleToken"));
  pileupInfoToken_ = consumes<std::vector<PileupSummaryInfo> >(iConfig.getUntrackedParameter<edm::InputTag>("pileupInfoToken"));
  
  l1GenData_ = std::make_unique<L1Analysis::L1AnalysisCustomGeneratorDataFormat>();

  // set up output
  tree_=fs_->make<TTree>("L1GenTree", "L1GenTree");
  tree_->Branch("Generator", "L1Analysis::L1AnalysisCustomGeneratorDataFormat", l1GenData_.get(), 32000, 3);
}


L1CustomGenTreeProducer::~L1CustomGenTreeProducer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
L1CustomGenTreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  l1GenData_->Reset();

  edm::Handle<reco::GenJetCollection> genJets;
  iEvent.getByToken(genJetToken_, genJets);


  if (genJets.isValid()){ 

    reco::GenJetCollection::const_iterator jetItr = genJets->begin();
    reco::GenJetCollection::const_iterator jetEnd = genJets->end();
    for( ; jetItr != jetEnd ; ++jetItr) {
      l1GenData_->jetPt.push_back( jetItr->pt() );
      l1GenData_->jetEta.push_back( jetItr->eta() );
      l1GenData_->jetPhi.push_back( jetItr->phi() );
      l1GenData_->nJet++;
    }

  } else {
    edm::LogWarning("MissingProduct") << "Gen jets not found. Branch will not be filled" << std::endl;
  }

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByToken(genParticleToken_, genParticles);

  if (genParticles.isValid()) {

    int nPart {0};

    for(size_t i = 0; i < genParticles->size(); ++ i) {
      const reco::GenParticle & p = (*genParticles)[i];
      
      // Get the mothers of the genParticle
      std::vector<unsigned short> mothers; //m
      unsigned int nMo=p.numberOfMothers();

      // For-loop: mothers 
      for(unsigned int iMom=0;iMom<nMo;++iMom){

	// Dynamic cast of reco::Candidate to reco::GenParticle                                                                                               
	const reco::GenParticle* mom = dynamic_cast<const reco::GenParticle*>( p.mother(iMom) ); //m
	//if (cfg_DebugMode) PrintGenParticle(*mom, iMom, "GenParticle Mothers", (iMom==0) );
	
	int diffMom   = &(p) - mom;
	int p_IMom = mom!=0 ? i-diffMom : 65535; // condition ? result1 : result2 (treat 65535 as NULL)                                           
	mothers.push_back(p_IMom);

	// Sanity check
	if(mom!=0)
	  {
	    const reco::GenParticle* momFromDiff = &( genParticles->at(p_IMom) );
	    if( mom->pt() != momFromDiff->pt() )
	      {
		edm::LogError("L1CustomGenTreeProducer") << "\nERROR! GenParticle mom pT = " << mom->pt() << ", genp_IMom pT = " << momFromDiff->pt() << std::endl;
	      }
	  }

      } // For-loop: mothers

      // Get the daughters of the genParticle
      std::vector<unsigned short> daughters;                                                                                                                  
      unsigned int nDau=p.numberOfDaughters();

      // For-loop: daughters
      for(unsigned int iDau=0;iDau<nDau;++iDau){
	
	// Dynamic cast of reco::Candidate to reco::GenParticle                                                                                             
	const reco::GenParticle* dau = dynamic_cast<const reco::GenParticle*>( p.daughter(iDau) );                                                     
	//if (cfg_DebugMode) PrintGenParticle(*dau, iDau, "GenParticle Daughters", (iDau==0) );                                                               
	
	int diffDau   = dau - &(p);                                                                                                                      
	int p_IDau = dau!=0 ? i+diffDau : 0; // condition ? result1 : result2                                                                   
	daughters.push_back( p_IDau );                                                                                                                   
	
	// Sanity check                                                                                                                                     
	if(dau!=0)                                                                                                                                          
	  {                                                                                                                                                 
	    const reco::GenParticle* dauFromDiff = &(genParticles->at(p_IDau) );                                                                  
	    if(dau->pt() != dauFromDiff->pt())                                                                                                              
	      {                                                                                                                                             
		edm::LogError("L1CustomGenTreeProducer") << "\nERROR! GenParticle daughter pT = " << dau->pt() << ", genp_IDau pT = " << dauFromDiff->pt() << std::endl;     
	      }                                                                                                                                             
	  }                                                                                                                                                 
      }// For-loop: daughters                               

      l1GenData_->partId.push_back(p.pdgId());
      l1GenData_->partStat.push_back(p.status());
      l1GenData_->partPt.push_back(p.pt());
      l1GenData_->partEta.push_back(p.eta());
      l1GenData_->partPhi.push_back(p.phi());
      l1GenData_->partMass.push_back(p.mass());
      l1GenData_->partE.push_back(p.energy());
      l1GenData_->partCh.push_back(p.charge());
      l1GenData_->partVertexX.push_back(p.vx());
      l1GenData_->partVertexY.push_back(p.vy());
      l1GenData_->partVertexZ.push_back(p.vz());
      l1GenData_->partMothers.push_back(mothers);
      l1GenData_->partDaughters.push_back(daughters);
      ++nPart;
            
    }
    l1GenData_->nPart = nPart;
  }


  edm::Handle<std::vector<PileupSummaryInfo>> puInfoCollection;
  iEvent.getByToken(pileupInfoToken_, puInfoCollection);

  if (!puInfoCollection.isValid()) {
    throw cms::Exception("ProductNotValid") << "pileupInfoSource not valid";
  }

  // Loop over vector, find in-time entry, then store the relevant info
  std::vector<PileupSummaryInfo>::const_iterator puItr = puInfoCollection->begin();
  std::vector<PileupSummaryInfo>::const_iterator puEnd = puInfoCollection->end();
  for( ; puItr != puEnd; ++puItr) {
    int bx = puItr->getBunchCrossing();
    if (bx == 0) {
      l1GenData_->nMeanPU = puItr->getTrueNumInteractions();
      l1GenData_->nVtx    = puItr->getPU_NumInteractions();
      break;
    }
  }



  tree_->Fill();

}

// ------------ method called once each job just before starting event loop  ------------
void 
L1CustomGenTreeProducer::beginJob(void)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1CustomGenTreeProducer::endJob() {
}

template<typename T> void L1CustomGenTreeProducer::print(T t, const int& width, const int& precision)
{
  const char separator = ' ';
  cout << setprecision(precision) << left << setw(width) << setfill(separator) << t;
}


void L1CustomGenTreeProducer::PrintGenParticle(reco::GenParticle genP, int genP_index, string title, bool printHeader)
{

  if (printHeader)
    {
      cout << string(75, ' ');
      cout << title << endl;
      cout << string(150, '=') << endl;
      print("Index", 8);
      print("Pt");
      print("Eta");
      print("Phi");
      print("Mass");
      print("Charge");
      print("PdgId");
      print("Status");
      print("vx");
      print("vy");
      print("vz");
      print("Mothers");
      print("Daughters");
      cout << endl;
      cout << string(150, '=') << endl;
    }

  float pt     = genP.pt();
  float eta    = genP.eta();
  float phi    = genP.phi();
  float mass   = genP.mass();
  int charge    = genP.charge();
  int pdgId     = genP.pdgId();
  int status    = genP.status();
  float vx     = genP.vx();
  float vy     = genP.vy();
  float vz     = genP.vz();
  int mothers   = genP.numberOfMothers();
  int daughters = genP.numberOfDaughters();

  // Print the variables                                                                                                                                       
  print(genP_index, 8);
  print(pt);
  print(eta);
  print(phi);
  print(mass);
  print(charge);
  print(pdgId);
  print(status);
  print(vx);
  print(vy);
  print(vz);
  print(mothers);
  print(daughters);
  cout << endl;

  return;
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1CustomGenTreeProducer);





