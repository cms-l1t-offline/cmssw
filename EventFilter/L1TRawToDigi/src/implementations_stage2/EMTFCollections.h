
#ifndef EMTFCollections_h
#define EMTFCollections_h

#include <iostream> // For use in all EMTFBlock files
#include <iomanip>  // For things like std::setw

#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/EMTFOutput.h"
#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1TMuon/interface/EMTFTrack.h"

#include "EventFilter/L1TRawToDigi/interface/UnpackerCollections.h"

namespace l1t {
  namespace stage2 {
    class EMTFCollections : public UnpackerCollections {
    public:
    EMTFCollections(edm::Event& e) :
      UnpackerCollections(e), // What are these? - AWB 27.01.16
	regionalMuonCands_(new RegionalMuonCandBxCollection()),
	EMTFOutputs_(new EMTFOutputCollection()),
	EMTFHits_(new EMTFHitCollection()),
	EMTFTracks_(new EMTFTrackCollection())
	  {};
      
      virtual ~EMTFCollections();
      
      inline RegionalMuonCandBxCollection* getRegionalMuonCands() { return regionalMuonCands_.get(); };
      // How does this work?  I haven't even defined a "get()" function for the EMTFOutputCollection. - AWB 28.01.16
      inline EMTFOutputCollection* getEMTFOutputs() { return EMTFOutputs_.get(); };       
      inline EMTFHitCollection*    getEMTFHits()    { return EMTFHits_.get();    };       
      inline EMTFTrackCollection*  getEMTFTracks()  { return EMTFTracks_.get();  };       
      
    private:
      
      std::auto_ptr<RegionalMuonCandBxCollection> regionalMuonCands_;
      std::auto_ptr<EMTFOutputCollection>         EMTFOutputs_;
      std::auto_ptr<EMTFHitCollection>            EMTFHits_;
      std::auto_ptr<EMTFTrackCollection>          EMTFTracks_;
      
    };
  }
}

#endif
