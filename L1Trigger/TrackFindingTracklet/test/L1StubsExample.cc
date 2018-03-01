//////////////////////////////////////////////////////////////////////
//                                                                  //
//  EXAMPLE of analyzer for stubs                                   //
//                                                                  //
//////////////////////////////////////////////////////////////////////

////////////////////
// FRAMEWORK HEADERS
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

///////////////////////
// DATA FORMATS HEADERS
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1TrackTrigger/interface/TTCluster.h"
#include "DataFormats/L1TrackTrigger/interface/TTStub.h"
#include "DataFormats/L1TrackTrigger/interface/TTTrack.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTClusterAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTStubAssociationMap.h"
#include "SimTracker/TrackTriggerAssociation/interface/TTTrackAssociationMap.h"
#include "Geometry/Records/interface/StackedTrackerGeometryRecord.h"

////////////////////////////
// DETECTOR GEOMETRY HEADERS
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelTopologyBuilder.h"
#include "Geometry/Records/interface/StackedTrackerGeometryRecord.h"

////////////////
// PHYSICS TOOLS
#include "CommonTools/UtilAlgos/interface/TFileService.h"

///////////////
// ROOT HEADERS
#include <TROOT.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>

//////////////
// STD HEADERS
#include <memory>
#include <string>
#include <iostream>

//////////////
// NAMESPACES
using namespace std;
using namespace edm;


//////////////////////////////
//                          //
//     CLASS DEFINITION     //
//                          //
//////////////////////////////

class L1StubsExample : public edm::EDAnalyzer
{
public:

  // Constructor/destructor
  explicit L1StubsExample(const edm::ParameterSet& iConfig);
  virtual ~L1StubsExample();

  // Mandatory methods
  virtual void beginJob();
  virtual void endJob();
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  
protected:
  
private:
  
  //-----------------------------------------------------------------------------------------------
  // Containers of parameters passed by python configuration file
  edm::ParameterSet config; 
  
  edm::InputTag MCTruthClusterInputTag;
  edm::InputTag L1StubInputTag;
  edm::InputTag MCTruthStubInputTag;
  edm::InputTag TrackingParticleInputTag;
  edm::InputTag TrackingVertexInputTag;

  edm::EDGetTokenT< edmNew::DetSetVector< TTCluster< Ref_Phase2TrackerDigi_ > > > ttClusterToken_;
  edm::EDGetTokenT< edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_ > > > ttStubToken_;
  edm::EDGetTokenT< TTClusterAssociationMap< Ref_Phase2TrackerDigi_ > > ttClusterMCTruthToken_;
  edm::EDGetTokenT< TTStubAssociationMap< Ref_Phase2TrackerDigi_ > > ttStubMCTruthToken_;

  edm::EDGetTokenT< std::vector< TrackingParticle > > TrackingParticleToken_;
  edm::EDGetTokenT< std::vector< TrackingVertex > > TrackingVertexToken_;


  //-----------------------------------------------------------------------------------------------
  // tree & branches for mini-ntuple

  TTree* eventTree;

  // TRACKING PARTICLES, saving those with: 
  // pt > 2 GeV, |eta| < 2.5, |z0| < 30 cm
  // and originates from the primary interaction (i.e. not from pileup events) 
  // and produces >= 1 cluster in the outer tracker 
  //
  std::vector<float>* m_tp_pt;        // pt
  std::vector<float>* m_tp_eta;       // eta
  std::vector<float>* m_tp_phi;       // phi
  std::vector<float>* m_tp_dxy;       // distance in transverse place of the tracking particle production point from the origin
  std::vector<float>* m_tp_d0;        // transverse impact parameter, propagated back to the IP 
  std::vector<float>* m_tp_z0;        // z0 position, i.e. longitudinal impact parameter, propagated back to the IP
  std::vector<float>* m_tp_d0_prod;   // "d0" but at the production point (stored for reference)
  std::vector<float>* m_tp_z0_prod;   // z coordinate of the production point of the tracking particle
  std::vector<int>*   m_tp_pdgid;     // PDG ID 
  std::vector<int>*   m_tp_nstub;     // # of stubs that can be associated to the tracking particle with at least one hit of at least one of its clusters 
                                      // (see: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SLHCTrackerTriggerSWTools#MC_truth_for_TTStub)
  std::vector<int>*   m_tp_eventid;   // event identifier, by default only eventid==0 are stored (corresponding to events from primary interaction), eventid > 0 means pileup

  // properties of ALL produced stubs
  std::vector<float>* m_allstub_x;               // global x coordinate
  std::vector<float>* m_allstub_y;               // global y coordinate
  std::vector<float>* m_allstub_z;               // global z coordinate
  std::vector<int>*   m_allstub_isBarrel;        // stub is in barrel (1) or in disk (0)
  std::vector<int>*   m_allstub_layer;           // layer (1-6) or disk (1-5) number

  // from: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SLHCTrackerTriggerSWTools#TTStub         
  //                                        
  std::vector<float>* m_allstub_trigDisplace;    // relative displacement between the two cluster centroids
  std::vector<float>* m_allstub_trigOffset;      // correction offset calculated while accepting/rejecting the stub
  std::vector<float>* m_allstub_trigPos;         // xy coordinate of the cluster centroid
  std::vector<float>* m_allstub_trigBend;        // corrected displacement between the cluster centroids

  // from: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SLHCTrackerTriggerSWTools#MC_truth_for_TTStub
  //
  // If both clusters are unknown, the stub is unknown
  // If only one cluster is unknown, the stub is combinatoric
  // If both clusters are genuine, and are associated to the same TrackingParticle, the stub is genuine
  // If both clusters are genuine, but are associated to different TrackingParticle 's, the stub is combinatoric
  // If both clusters are not-unknown, and at least one of them is combinatoric, one unique TrackingParticle must be associated to both in order to have a genuine stub, otherwise the stub will be combinatoric 
  // 
  std::vector<int>*   m_allstub_isGenuine;      
  std::vector<int>*   m_allstub_isCombinatoric; 
  std::vector<int>*   m_allstub_isUnknown;      


};


//////////////////////////////////
//                              //
//     CLASS IMPLEMENTATION     //
//                              //
//////////////////////////////////

//////////////
// CONSTRUCTOR
L1StubsExample::L1StubsExample(edm::ParameterSet const& iConfig) : 
  config(iConfig)
{
  L1StubInputTag      = iConfig.getParameter<edm::InputTag>("L1StubInputTag");
  MCTruthClusterInputTag = iConfig.getParameter<edm::InputTag>("MCTruthClusterInputTag");
  MCTruthStubInputTag = iConfig.getParameter<edm::InputTag>("MCTruthStubInputTag");
  TrackingParticleInputTag = iConfig.getParameter<edm::InputTag>("TrackingParticleInputTag");
  TrackingVertexInputTag = iConfig.getParameter<edm::InputTag>("TrackingVertexInputTag");

  ttStubToken_ = consumes< edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_ > > >(L1StubInputTag);
  ttClusterMCTruthToken_ = consumes< TTClusterAssociationMap< Ref_Phase2TrackerDigi_ > >(MCTruthClusterInputTag);
  ttStubMCTruthToken_ = consumes< TTStubAssociationMap< Ref_Phase2TrackerDigi_ > >(MCTruthStubInputTag);
  TrackingParticleToken_ = consumes< std::vector< TrackingParticle > >(TrackingParticleInputTag);
  TrackingVertexToken_ = consumes< std::vector< TrackingVertex > >(TrackingVertexInputTag);
}

/////////////
// DESTRUCTOR
L1StubsExample::~L1StubsExample()
{
}  

//////////
// END JOB
void L1StubsExample::endJob()
{
  // things to be done at the exit of the event Loop
  cerr << "L1StubsExample::endJob" << endl;

}

////////////
// BEGIN JOB
void L1StubsExample::beginJob()
{

  // things to be done before entering the event Loop
  cerr << "L1StubsExample::beginJob" << endl;

  //-----------------------------------------------------------------------------------------------
  // book histograms / make ntuple
  edm::Service<TFileService> fs;


  // initilize
  m_tp_pt     = new std::vector<float>;
  m_tp_eta    = new std::vector<float>;
  m_tp_phi    = new std::vector<float>;
  m_tp_dxy    = new std::vector<float>;
  m_tp_d0     = new std::vector<float>;
  m_tp_z0     = new std::vector<float>;
  m_tp_d0_prod = new std::vector<float>;
  m_tp_z0_prod = new std::vector<float>;
  m_tp_pdgid  = new std::vector<int>;
  m_tp_nstub  = new std::vector<int>;
  m_tp_eventid = new std::vector<int>;

  m_allstub_x = new std::vector<float>;
  m_allstub_y = new std::vector<float>;
  m_allstub_z = new std::vector<float>;
  m_allstub_isBarrel = new std::vector<int>;
  m_allstub_layer    = new std::vector<int>;
  m_allstub_trigDisplace = new std::vector<float>;
  m_allstub_trigOffset   = new std::vector<float>;
  m_allstub_trigPos      = new std::vector<float>;
  m_allstub_trigBend     = new std::vector<float>;
  m_allstub_isGenuine = new std::vector<int>;
  m_allstub_isCombinatoric = new std::vector<int>;
  m_allstub_isUnknown = new std::vector<int>;


  // ntuple
  eventTree = fs->make<TTree>("eventTree", "Event tree");

  eventTree->Branch("tp_pt",     &m_tp_pt);
  eventTree->Branch("tp_eta",    &m_tp_eta);
  eventTree->Branch("tp_phi",    &m_tp_phi);
  eventTree->Branch("tp_dxy",    &m_tp_dxy);
  eventTree->Branch("tp_d0",     &m_tp_d0);
  eventTree->Branch("tp_z0",     &m_tp_z0);
  eventTree->Branch("tp_d0_prod",&m_tp_d0_prod);
  eventTree->Branch("tp_z0_prod",&m_tp_z0_prod);
  eventTree->Branch("tp_pdgid",  &m_tp_pdgid);
  eventTree->Branch("tp_nstub", &m_tp_nstub);
  eventTree->Branch("tp_eventid",&m_tp_eventid);

  eventTree->Branch("allstub_x", &m_allstub_x);
  eventTree->Branch("allstub_y", &m_allstub_y);
  eventTree->Branch("allstub_z", &m_allstub_z);
  eventTree->Branch("allstub_isBarrel", &m_allstub_isBarrel);
  eventTree->Branch("allstub_layer",    &m_allstub_layer);
  eventTree->Branch("allstub_trigDisplace", &m_allstub_trigDisplace);
  eventTree->Branch("allstub_trigOffset", &m_allstub_trigOffset);
  eventTree->Branch("allstub_trigPos", &m_allstub_trigPos);
  eventTree->Branch("allstub_trigBend", &m_allstub_trigBend);
  eventTree->Branch("allstub_isGenuine", &m_allstub_isGenuine);
  eventTree->Branch("allstub_isCombinatoric", &m_allstub_isCombinatoric);
  eventTree->Branch("allstub_isUnknown", &m_allstub_isUnknown);

}


//////////
// ANALYZE
void L1StubsExample::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  m_tp_pt->clear();
  m_tp_eta->clear();
  m_tp_phi->clear();
  m_tp_dxy->clear();
  m_tp_d0->clear();
  m_tp_z0->clear();
  m_tp_d0_prod->clear();
  m_tp_z0_prod->clear();
  m_tp_pdgid->clear();
  m_tp_nstub->clear();
  m_tp_eventid->clear();

  m_allstub_x->clear();
  m_allstub_y->clear();
  m_allstub_z->clear();
  m_allstub_isBarrel->clear();
  m_allstub_layer->clear();
  m_allstub_trigDisplace->clear();
  m_allstub_trigOffset->clear();
  m_allstub_trigPos->clear();
  m_allstub_trigBend->clear();
  m_allstub_isGenuine->clear();
  m_allstub_isCombinatoric->clear();
  m_allstub_isUnknown->clear();


  // -----------------------------------------------------------------------------------------------
  // retrieve various containers
  // -----------------------------------------------------------------------------------------------

  // L1 stubs
  edm::Handle< edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_ > > > TTStubHandle;
  iEvent.getByToken(ttStubToken_, TTStubHandle);

  // MC truth association maps
  edm::Handle< TTClusterAssociationMap< Ref_Phase2TrackerDigi_ > > MCTruthTTClusterHandle;
  iEvent.getByToken(ttClusterMCTruthToken_, MCTruthTTClusterHandle);
  edm::Handle< TTStubAssociationMap< Ref_Phase2TrackerDigi_ > > MCTruthTTStubHandle;
  iEvent.getByToken(ttStubMCTruthToken_, MCTruthTTStubHandle);

  // tracking particles
  edm::Handle< std::vector< TrackingParticle > > TrackingParticleHandle;
  edm::Handle< std::vector< TrackingVertex > > TrackingVertexHandle;
  iEvent.getByToken(TrackingParticleToken_, TrackingParticleHandle);
  iEvent.getByToken(TrackingVertexToken_, TrackingVertexHandle);

  // geometry handles
  edm::ESHandle<TrackerGeometry> geometryHandle;
  iSetup.get<TrackerDigiGeometryRecord>().get(geometryHandle);

  edm::ESHandle<TrackerTopology> tTopoHandle;
  iSetup.get<TrackerTopologyRcd>().get(tTopoHandle);

  edm::ESHandle<TrackerGeometry> tGeomHandle;
  iSetup.get<TrackerDigiGeometryRecord>().get(tGeomHandle);

  const TrackerTopology* const tTopo = tTopoHandle.product();
  const TrackerGeometry* const theTrackerGeom = tGeomHandle.product();


  // ----------------------------------------------------------------------------------------------
  // loop over L1 stubs
  // ----------------------------------------------------------------------------------------------

  for (auto gd=theTrackerGeom->dets().begin(); gd != theTrackerGeom->dets().end(); gd++) {
    
    DetId detid = (*gd)->geographicalId();
    if(detid.subdetId()!=StripSubdetector::TOB && detid.subdetId()!=StripSubdetector::TID ) continue;
    if(!tTopo->isLower(detid) ) continue; // loop on the stacks: choose the lower arbitrarily
    DetId stackDetid = tTopo->stack(detid); // Stub module detid
    
    if (TTStubHandle->find( stackDetid ) == TTStubHandle->end() ) continue;
    
    // Get the DetSets of the Clusters
    edmNew::DetSet< TTStub< Ref_Phase2TrackerDigi_ > > stubs = (*TTStubHandle)[ stackDetid ];
    const GeomDetUnit* det0 = theTrackerGeom->idToDetUnit( detid );
    const PixelGeomDetUnit* theGeomDet = dynamic_cast< const PixelGeomDetUnit* >( det0 );
    const PixelTopology* topol = dynamic_cast< const PixelTopology* >( &(theGeomDet->specificTopology()) );
    
    // loop over stubs
    for ( auto stubIter = stubs.begin();stubIter != stubs.end();++stubIter ) {
      edm::Ref< edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_  > >, TTStub< Ref_Phase2TrackerDigi_  > >
	tempStubPtr = edmNew::makeRefTo( TTStubHandle, stubIter );
      
      MeasurementPoint coords = tempStubPtr->getClusterRef(0)->findAverageLocalCoordinatesCentered();      
      LocalPoint clustlp = topol->localPosition(coords);
      
      GlobalPoint posStub  =  theGeomDet->surface().toGlobal(clustlp);
      
      double tmp_stub_x=posStub.x();
      double tmp_stub_y=posStub.y();
      double tmp_stub_z=posStub.z();
      
      int layer=-999999;
      int isBarrel = 0;
      if ( detid.subdetId()==StripSubdetector::TOB ) {
	isBarrel = 1;
	layer  = static_cast<int>(tTopo->layer(detid));
      }
      else if ( detid.subdetId()==StripSubdetector::TID ) {
	isBarrel = 0;
	layer  = static_cast<int>(tTopo->layer(detid));
      }
      else {
	cout << "WARNING -- neither TOB or TID stub, shouldn't happen..." << endl;
	layer = -1;
      }
      
      
      float trigDisplace = tempStubPtr->getTriggerDisplacement();
      float trigOffset = tempStubPtr->getTriggerOffset();
      float trigPos = tempStubPtr->getTriggerPosition();
      float trigBend = tempStubPtr->getTriggerBend();

      int isGenuine = (int) MCTruthTTStubHandle->isGenuine(tempStubPtr);
      int isCombinatoric = (int) MCTruthTTStubHandle->isCombinatoric(tempStubPtr);
      int isUnknown = (int) MCTruthTTStubHandle->isUnknown(tempStubPtr);
      
      m_allstub_x->push_back(tmp_stub_x);
      m_allstub_y->push_back(tmp_stub_y);
      m_allstub_z->push_back(tmp_stub_z);
      m_allstub_isBarrel->push_back(isBarrel);
      m_allstub_layer->push_back(layer);
      
      m_allstub_trigDisplace->push_back(trigDisplace);
      m_allstub_trigOffset->push_back(trigOffset);
      m_allstub_trigPos->push_back(trigPos);
      m_allstub_trigBend->push_back(trigBend);

      m_allstub_isGenuine->push_back(isGenuine);
      m_allstub_isCombinatoric->push_back(isCombinatoric);
      m_allstub_isUnknown->push_back(isUnknown);
      
    }
  }



  // ----------------------------------------------------------------------------------------------
  // loop over tracking particles
  // ----------------------------------------------------------------------------------------------

  int this_tp = 0;
  std::vector< TrackingParticle >::const_iterator iterTP;
  for (iterTP = TrackingParticleHandle->begin(); iterTP != TrackingParticleHandle->end(); ++iterTP) {
 
    edm::Ptr< TrackingParticle > tp_ptr(TrackingParticleHandle, this_tp);
    this_tp++;

    int tmp_eventid = iterTP->eventId().event();
    if (tmp_eventid > 0) continue; //only store TPs from the primary interaction

    float tmp_tp_pt  = iterTP->pt();
    float tmp_tp_eta = iterTP->eta();
    float tmp_tp_phi = iterTP->phi(); 
    float tmp_tp_vz  = iterTP->vz();
    float tmp_tp_vx  = iterTP->vx();
    float tmp_tp_vy  = iterTP->vy();
    int tmp_tp_pdgid = iterTP->pdgId();
    float tmp_tp_z0_prod = tmp_tp_vz;
    float tmp_tp_d0_prod = -tmp_tp_vx*sin(tmp_tp_phi) + tmp_tp_vy*cos(tmp_tp_phi);

    if (tmp_tp_pt < 2.0) continue;
    if (fabs(tmp_tp_eta) > 2.5) continue;


    // ----------------------------------------------------------------------------------------------
    // get d0/z0 propagated back to the IP

    float tmp_tp_t = tan(2.0*atan(1.0)-2.0*atan(exp(-tmp_tp_eta)));

    float delx = -tmp_tp_vx;
    float dely = -tmp_tp_vy;
	
    float A = 0.01*0.5696;
    float Kmagnitude = A / tmp_tp_pt;
    
    float tmp_tp_charge = tp_ptr->charge();
    float K = Kmagnitude * tmp_tp_charge;
    float d = 0;
	
    float tmp_tp_x0p = delx - (d + 1./(2. * K)*sin(tmp_tp_phi));
    float tmp_tp_y0p = dely + (d + 1./(2. * K)*cos(tmp_tp_phi));
    float tmp_tp_rp = sqrt(tmp_tp_x0p*tmp_tp_x0p + tmp_tp_y0p*tmp_tp_y0p);
    float tmp_tp_d0 = tmp_tp_charge*tmp_tp_rp - (1. / (2. * K));
	
    tmp_tp_d0 = tmp_tp_d0*(-1); //fix d0 sign

    static double pi = 4.0*atan(1.0);
    float delphi = tmp_tp_phi-atan2(-K*tmp_tp_x0p,K*tmp_tp_y0p);
    if (delphi<-pi) delphi+=2.0*pi;
    if (delphi>pi) delphi-=2.0*pi;
    float tmp_tp_z0 = tmp_tp_vz+tmp_tp_t*delphi/(2.0*K);
    // ----------------------------------------------------------------------------------------------
    
    if (fabs(tmp_tp_z0) > 30.0) continue;


    // for pions in ttbar, only consider TPs coming from near the IP!
    float dxy = sqrt(tmp_tp_vx*tmp_tp_vx + tmp_tp_vy*tmp_tp_vy);
    float tmp_tp_dxy = dxy;


    // ----------------------------------------------------------------------------------------------
    // only consider TPs associated with >= 1 cluster
    
    if (MCTruthTTClusterHandle->findTTClusterRefs(tp_ptr).size() < 1) continue;

    std::vector< edm::Ref< edmNew::DetSetVector< TTStub< Ref_Phase2TrackerDigi_ > >, TTStub< Ref_Phase2TrackerDigi_ > > > theStubRefs = MCTruthTTStubHandle->findTTStubRefs(tp_ptr);
    int nStubTP = (int) theStubRefs.size(); 


    m_tp_pt->push_back(tmp_tp_pt);
    m_tp_eta->push_back(tmp_tp_eta);
    m_tp_phi->push_back(tmp_tp_phi);
    m_tp_dxy->push_back(tmp_tp_dxy);
    m_tp_z0->push_back(tmp_tp_z0);
    m_tp_d0->push_back(tmp_tp_d0);
    m_tp_z0_prod->push_back(tmp_tp_z0_prod);
    m_tp_d0_prod->push_back(tmp_tp_d0_prod);
    m_tp_pdgid->push_back(tmp_tp_pdgid);
    m_tp_nstub->push_back(nStubTP);
    m_tp_eventid->push_back(tmp_eventid);

  } //end loop tracking particles
  

  eventTree->Fill();


} // end of analyze()


///////////////////////////
// DEFINE THIS AS A PLUG-IN
DEFINE_FWK_MODULE(L1StubsExample);
