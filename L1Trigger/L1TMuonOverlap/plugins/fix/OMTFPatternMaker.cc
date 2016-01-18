#include <iostream>

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "L1Trigger/L1TMuonOverlap/plugins/OMTFPatternMaker.h"
#include "L1Trigger/L1TMuonOverlap/interface/OMTFProcessor.h"
#include "L1Trigger/L1TMuonOverlap/interface/OMTFinputMaker.h"
#include "L1Trigger/L1TMuonOverlap/interface/OMTFinput.h"
#include "L1Trigger/L1TMuonOverlap/interface/OMTFConfiguration.h"
#include "L1Trigger/L1TMuonOverlap/interface/OMTFConfigMaker.h"
#include "L1Trigger/L1TMuonOverlap/interface/XMLConfigWriter.h"

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include "Math/VectorUtil.h"

OMTFPatternMaker::OMTFPatternMaker(const edm::ParameterSet& cfg):
  theConfig(cfg),
  g4SimTrackSrc(cfg.getParameter<edm::InputTag>("g4SimTrackSrc")){

  inputTokenDTPh = consumes<L1MuDTChambPhContainer>(theConfig.getParameter<edm::InputTag>("srcDTPh"));
  inputTokenDTTh = consumes<L1MuDTChambThContainer>(theConfig.getParameter<edm::InputTag>("srcDTTh"));
  inputTokenCSC = consumes<CSCCorrelatedLCTDigiCollection>(theConfig.getParameter<edm::InputTag>("srcCSC"));
  inputTokenRPC = consumes<RPCDigiCollection>(theConfig.getParameter<edm::InputTag>("srcRPC"));
    
  if(!theConfig.exists("omtf")){
    edm::LogError("OMTFPatternMaker")<<"omtf configuration not found in cfg.py";
  }
  
  myInputMaker = new OMTFinputMaker();
  
  myWriter = new XMLConfigWriter();
  std::string fName = "OMTF";
  myWriter->initialiseXMLDocument(fName);

  makeGoldenPatterns = theConfig.getParameter<bool>("makeGoldenPatterns");
  makeConnectionsMaps = theConfig.getParameter<bool>("makeConnectionsMaps");

  myOMTFConfig = 0;
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
OMTFPatternMaker::~OMTFPatternMaker(){

  delete myOMTFConfig;
  delete myOMTFConfigMaker;
  delete myOMTF;

}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
void OMTFPatternMaker::beginJob(){

  if(theConfig.exists("omtf")){
    myOMTFConfig = new OMTFConfiguration(theConfig.getParameter<edm::ParameterSet>("omtf"));
    myOMTFConfigMaker = new OMTFConfigMaker(theConfig.getParameter<edm::ParameterSet>("omtf"));
    myOMTF = new OMTFProcessor(theConfig.getParameter<edm::ParameterSet>("omtf"));
  }


  ///For making the patterns use extended pdf width in phi
  ////Ugly hack to modify confoguration parameters at runtime.
  OMTFConfiguration::nPdfAddrBits = 14;

  ///Clear existing GoldenPatterns
  const std::map<Key,GoldenPattern*> & theGPs = myOMTF->getPatterns();
  for(auto itGP: theGPs) itGP.second->reset();

}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////  
void OMTFPatternMaker::endJob(){

  if(makeGoldenPatterns && !makeConnectionsMaps){
    std::string fName = "OMTF";
    myWriter->initialiseXMLDocument(fName);
    const std::map<Key,GoldenPattern*> & myGPmap = myOMTF->getPatterns();
    for(auto itGP: myGPmap){
      if(!itGP.second->hasCounts()) continue;
      itGP.second->normalise();
    }

    ////Ugly hack to modify configuration parameters at runtime.
    OMTFConfiguration::nPdfAddrBits = 7;
    for(auto itGP: myGPmap){
      if((int)itGP.first.thePtCode==theConfig.getParameter<int>("ptCode") && 
	 itGP.first.theCharge==theConfig.getParameter<int>("charge")){ 
	std::cout<<*itGP.second<<std::endl;      
	myWriter->writeGPData(*itGP.second);
      }
    }
    fName = "GPs.xml";
    myWriter->finaliseXMLDocument(fName);        
  }

  if(makeConnectionsMaps && !makeGoldenPatterns){
    std::string fName = "Connections.xml";
    unsigned int iProcessor = 0;
    ///Order important: printPhiMap updates global vector in OMTFConfiguration
    myOMTFConfigMaker->printPhiMap(std::cout);
    myOMTFConfigMaker->printConnections(std::cout,iProcessor,5);
    myOMTFConfigMaker->printConnections(std::cout,1,0);


    /*
    myOMTFConfigMaker->printConnections(std::cout,iProcessor,1);
    myOMTFConfigMaker->printConnections(std::cout,iProcessor,2);
    myOMTFConfigMaker->printConnections(std::cout,iProcessor,3);
    myOMTFConfigMaker->printConnections(std::cout,iProcessor,4);
    myOMTFConfigMaker->printConnections(std::cout,iProcessor,5);
    */
    myWriter->writeConnectionsData(OMTFConfiguration::measurements4D);
    myWriter->finaliseXMLDocument(fName);
  }
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////  
void OMTFPatternMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup){

 ///Get the simulated muon parameters
  const SimTrack* aSimMuon = findSimMuon(iEvent,evSetup);
  if(!aSimMuon){
    edm::LogError("OMTFPatternMaker")<<"No SimMuon found in the event!";
    return;
  }
  
  myInputMaker->initialize(evSetup);

  edm::Handle<L1MuDTChambPhContainer> dtPhDigis;
  edm::Handle<L1MuDTChambThContainer> dtThDigis;
  edm::Handle<CSCCorrelatedLCTDigiCollection> cscDigis;
  edm::Handle<RPCDigiCollection> rpcDigis;
  
  ///Filter digis by dropping digis from selected (by cfg.py) subsystems
  if(!theConfig.getParameter<bool>("dropDTPrimitives")){
    iEvent.getByToken(inputTokenDTPh,dtPhDigis);
    iEvent.getByToken(inputTokenDTTh,dtThDigis);
  }
  if(!theConfig.getParameter<bool>("dropRPCPrimitives")) iEvent.getByToken(inputTokenRPC,rpcDigis);  
  if(!theConfig.getParameter<bool>("dropCSCPrimitives")) iEvent.getByToken(inputTokenCSC,cscDigis);

  //l1t::tftype mtfType = l1t::tftype::bmtf;
  l1t::tftype mtfType = l1t::tftype::omtf_pos;
  //l1t::tftype mtfType = l1t::tftype::emtf_pos;
  
  ///Loop over all processors, each covering 60 deg in phi
  for(unsigned int iProcessor=0;iProcessor<6;++iProcessor){
        
    ///Input data with phi ranges shifted for each processor, so it fits 11 bits range
    const OMTFinput *myInput = myInputMaker->buildInputForProcessor(dtPhDigis.product(),
								    dtThDigis.product(),
								    cscDigis.product(),
								    rpcDigis.product(),								       
								    iProcessor,
								    mtfType);
    
    ///Input data with phi ranges shifted for each processor, so it fits 10 bits range
    const OMTFinput myShiftedInput =  myOMTF->shiftInput(iProcessor,*myInput);	
    
    ///Phi maps should be made with original, global phi values.
    if(makeConnectionsMaps) myOMTFConfigMaker->makeConnetionsMap(iProcessor,*myInput);
  
    if(makeGoldenPatterns) myOMTF->fillCounts(iProcessor,myShiftedInput, aSimMuon);
    
  }
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////  
const SimTrack * OMTFPatternMaker::findSimMuon(const edm::Event &ev, const edm::EventSetup &es, const SimTrack * previous){

  const SimTrack * result = 0;
  edm::Handle<edm::SimTrackContainer> simTks;
  ev.getByLabel("g4SimHits",simTks);

  for (std::vector<SimTrack>::const_iterator it=simTks->begin(); it< simTks->end(); it++) {
    const SimTrack & aTrack = *it;
    if ( !(aTrack.type() == 13 || aTrack.type() == -13) )continue;
    if(previous && ROOT::Math::VectorUtil::DeltaR(aTrack.momentum(),previous->momentum())<0.07) continue;
    if ( !result || aTrack.momentum().pt() > result->momentum().pt()) result = &aTrack;
  }
  return result;
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////  
