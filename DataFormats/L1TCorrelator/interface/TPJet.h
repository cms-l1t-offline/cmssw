#ifndef DataFormatsL1TCorrelator_TPJet_h
#define DataFormatsL1TCorrelator_TPJet_h

// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     TPJet
//

#include "DataFormats/L1Trigger/interface/L1Candidate.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/L1Trigger/interface/Jet.h"
/* #include "DataFormats/L1TrackTrigger/interface/TTTypes.h" */
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

namespace l1t {

  class TPJet : public L1Candidate {
  public:
    /* typedef TTTrack<Ref_Phase2TrackerDigi_> L1TTTrackType; */
    /* typedef std::vector<L1TTTrackType> L1TTTrackCollection; */
    /* typedef std::vector<edm::Ptr<TrackingParticle>> L1TrackingParticlePtrs; */

    TPJet();

    TPJet(const LorentzVector& p4,
          const edm::Ref<JetBxCollection>& jetRef,
          const std::vector<edm::Ptr<TrackingParticle> >& L1TrackingParticlePtrs,
          float jetvtx = -999.);
    TPJet(const LorentzVector& p4,
          const std::vector<edm::Ptr<TrackingParticle> >& L1TrackingParticlePtrs,
          float jetvtx = -999.,
          unsigned int ntracks = 0,
          unsigned int tighttracks = 0,
          unsigned int displacedtracks = 0,
          unsigned int tightdisplacedtracks = 0,
          bool displacedTag = false);
    /*    TkJet(const LorentzVector& p4,
          const std::vector<edm::Ptr<L1TTTrackType> >& trkPtrs,
          float jetvtx = -999.,
          unsigned int ntracks = 0,
          unsigned int tighttracks = 0,
          unsigned int displacedtracks = 0,
          unsigned int tightdisplacedtracks = 0,
          bool displacedTag = false );*/

    // ---------- const member functions ---------------------

    const edm::Ref<JetBxCollection>& jetRef() const { return jetRef_; }

    const std::vector<edm::Ptr<TrackingParticle> >& L1TrackingParticlePtrs() const { return L1TrackingParticlePtrs_; }

    float jetVtx() const { return JetVtx_; }
    unsigned int ntracks() const { return ntracks_; }
    unsigned int nTighttracks() const { return tighttracks_; }
    unsigned int nDisptracks() const { return displacedtracks_; }
    unsigned int nTightDisptracks() const { return tightdisplacedtracks_; }
    bool isDisplaced() const { return displacedTag_; }

    // ---------- member functions ---------------------------
    void setJetVtx(float JetVtx) { JetVtx_ = JetVtx; }

    int bx() const;

  private:
    edm::Ref<JetBxCollection> jetRef_;
    std::vector<edm::Ptr<TrackingParticle>> L1TrackingParticlePtrs_;
    float JetVtx_;
    unsigned int ntracks_, tighttracks_, displacedtracks_, tightdisplacedtracks_;
    bool displacedTag_;
  };
}  // namespace l1t

#endif
