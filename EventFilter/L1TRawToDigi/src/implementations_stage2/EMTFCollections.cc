#include "FWCore/Framework/interface/Event.h"
#include "EMTFCollections.h"

namespace l1t {
  namespace stage2 {
    EMTFCollections::~EMTFCollections()
    {
      // std::cout << "Inside EMTFCollections.cc: ~EMTFCollections" << std::endl;
      event_.put(regionalMuonCands_);
      event_.put(EMTFOutputs_);
      event_.put(EMTFHits_);
      event_.put(EMTFTracks_);
    }
  }
}
