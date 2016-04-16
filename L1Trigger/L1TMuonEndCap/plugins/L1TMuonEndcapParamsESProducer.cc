// -*- C++ -*-
//
// Class:      L1TMuonEndcapParamsESProducer
//
// Original Author:  Matthew Carver UF
//         Created:
//
//


// system include files
#include <memory>
#include "boost/shared_ptr.hpp"

// user include files
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESProducts.h"

#include "CondFormats/L1TObjects/interface/L1TMuonEndcapParams.h"
#include "CondFormats/DataRecord/interface/L1TMuonEndcapParamsRcd.h"
#include "CondFormats/L1TObjects/interface/L1TMuEndCapForest_helper.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "TXMLEngine.h"

// class declaration

class L1TMuonEndcapParamsESProducer : public edm::ESProducer {
   public:
      L1TMuonEndcapParamsESProducer(const edm::ParameterSet&);
      ~L1TMuonEndcapParamsESProducer();
   
      typedef boost::shared_ptr<L1TMuonEndcapParams> ReturnType;

      ReturnType produce(const L1TMuonEndcapParamsRcd&);
   private:
      L1TMuonEndcapParams m_params;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1TMuonEndcapParamsESProducer::L1TMuonEndcapParamsESProducer(const edm::ParameterSet& iConfig)
{
   //the following line is needed to tell the framework what
   // data is being produced
   setWhatProduced(this);

   m_params.SetPtAssignVersion(iConfig.getParameter<int>("PtAssignVersion"));
   m_params.SetSt1PhiMatchWindow(iConfig.getParameter<int>("St1MatchWindow"));
   m_params.SetSt2PhiMatchWindow(iConfig.getParameter<int>("St2MatchWindow"));
   m_params.SetSt3PhiMatchWindow(iConfig.getParameter<int>("St3MatchWindow"));
   m_params.SetSt4PhiMatchWindow(iConfig.getParameter<int>("St4MatchWindow"));
   
   
   int allowedModes[11] = {3,5,9,6,10,12,7,11,13,14,15};
   std::vector<std::pair<int,EndCapSubForest*>> Forests;
   for(int modes=0;modes<11;modes++){
   
   		EndCapSubForest *forest = new EndCapSubForest();
		std::stringstream ss;
		ss << "L1Trigger/L1TMuon/data/emtf_luts/ModeVariables_v1_dTheta/trees/" << allowedModes[modes];
		forest->loadForestFromXML(ss.str().c_str(),64);
		std::pair<int,EndCapSubForest*> outPair(allowedModes[modes],forest);
		Forests.push_back(outPair);
   
   }
   m_params.SetPtForests(Forests);
   

}


L1TMuonEndcapParamsESProducer::~L1TMuonEndcapParamsESProducer()
{
}



//
// member functions
//

// ------------ method called to produce the data  ------------
L1TMuonEndcapParamsESProducer::ReturnType
L1TMuonEndcapParamsESProducer::produce(const L1TMuonEndcapParamsRcd& iRecord)
{
   using namespace edm::es;
   boost::shared_ptr<L1TMuonEndcapParams> pEMTFParams;

   pEMTFParams = boost::shared_ptr<L1TMuonEndcapParams>(new L1TMuonEndcapParams(m_params));
   return pEMTFParams;
   
}

//define this as a plug-in
DEFINE_FWK_EVENTSETUP_MODULE(L1TMuonEndcapParamsESProducer);
