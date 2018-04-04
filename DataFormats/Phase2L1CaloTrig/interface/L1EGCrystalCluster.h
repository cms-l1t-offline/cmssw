#ifndef L1EGammaCrystalsCluster_h
#define L1EGammaCrystalsCluster_h

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "DataFormats/L1Trigger/interface/L1Candidate.h"
#include "DataFormats/DetId/interface/DetId.h"

namespace l1slhc
{

  class L1EGCrystalCluster : public l1t::L1Candidate {
    public:
      L1EGCrystalCluster() : l1t::L1Candidate(), calibratedPt_(0.), hovere_(0.), iso_(0.), PUcorrPt_(0.), bremStrength_(0.),
            e2x2_(0.), e2x5_(0.), e3x5_(0.), e5x5_(0.), electronWP98_(0.), photonWP80_(0.), electronWP90_(0.),
            looseL1TkMatchWP_(0.), stage2effMatch_(0.) {};

      L1EGCrystalCluster(const PolarLorentzVector& p4, float calibratedPt, float hovere, float iso, DetId seedCrystal, 
            float PUcorrPt = 0., float bremStrength = 0., float e2x2 = 0., float e2x5 = 0.,
            float e3x5 = 0., float e5x5 = 0., bool electronWP98 = false, bool photonWP80 = false,
            bool electronWP90 = false, bool looseL1TkMatchWP = false, bool stage2effMatch = false ) :
                    l1t::L1Candidate(p4), calibratedPt_(calibratedPt), hovere_(hovere), iso_(iso), seedCrystal_(seedCrystal),
                    PUcorrPt_(PUcorrPt), bremStrength_(bremStrength), e2x2_(e2x2), e2x5_(e2x5),
                    e3x5_(e3x5), e5x5_(e5x5), electronWP98_(electronWP98), photonWP80_(photonWP80),
                    electronWP90_(electronWP90), looseL1TkMatchWP_(looseL1TkMatchWP),
                    stage2effMatch_(stage2effMatch) {};

      virtual ~L1EGCrystalCluster() {};
      inline float calibratedPt() const { return calibratedPt_; };
      inline float hovere() const { return hovere_; };
      inline float isolation() const { return iso_; };
      inline float PUcorrPt() const { return PUcorrPt_; };
      inline float bremStrength() const { return bremStrength_; };
      inline DetId seedCrystal() const { return seedCrystal_; };
      void SetCrystalPtInfo(std::vector<float> &info) {
         std::sort(info.begin(), info.end());
         std::reverse(info.begin(), info.end());
         crystalPt_ = info;
      };
      void SetExperimentalParams(const std::map<std::string, float> &params) { experimentalParams_ = params; };
      const std::map<std::string, float> GetExperimentalParams() const { return experimentalParams_; };
      inline float GetExperimentalParam(std::string name) const { return experimentalParams_.at(name); };

      inline float e2x2() const { return e2x2_; };
      inline float e2x5() const { return e2x5_; };
      inline float e3x5() const { return e3x5_; };
      inline float e5x5() const { return e5x5_; };
      inline float electronWP98() const { return electronWP98_; };
      inline float photonWP80() const { return photonWP80_; };
      inline float electronWP90() const { return electronWP90_; };
      inline float looseL1TkMatchWP() const { return looseL1TkMatchWP_; };
      inline float stage2effMatch() const { return stage2effMatch_; };

      // The index range depends on the algorithm eta,phi window, currently 3x5
      // The pt should always be ordered.
      inline float GetCrystalPt(unsigned int index) const { return (index < crystalPt_.size()) ? crystalPt_[index] : 0.; };
    
    private:
      // pT calibrated to Stage-2 (Phase-I) L1EG Objects.  NOTE
      // all working points are defined with respect to cluster.pt(),
      // not cluster.calibratedPt()
      float calibratedPt_;
      // HCal energy in region behind cluster (for size, look in producer) / ECal energy in cluster
      float hovere_;
      // ECal isolation (for outer window size, again look in producer)
      float iso_;
      // DetId of seed crystal used to make cluster (could be EBDetId or EEDetId)
      DetId seedCrystal_;
      // Pileup-corrected energy deposit, not studied carefully yet, don't use
      float PUcorrPt_;
      // Bremstrahlung strength, should be proportional to the likelihood of a brem.
      float bremStrength_;
      // Shower shape variable - max 2x2 energy containing seed crystal
      float e2x2_;
      // Shower shape variable - max 2x5 energy containing seed crystal, phi centered
      float e2x5_;
      // Shower shape variable - 3x5 energy containing seed crystal, phi centered
      float e3x5_;
      // Shower shape variable - 5x5 energy containing centered on seed crystal
      float e5x5_;
      // 98% efficient electron WP, for electrons above 35 GeV
      bool electronWP98_;
      // 80% efficient photon WP, for photons above 35 GeV
      bool photonWP80_;
      // 90% efficient electron based WP, early rise to efficiency plateau
      bool electronWP90_;
      // loose isolation and shower shape requirements to be used in conjunction with L1Trk matching
      bool looseL1TkMatchWP_;
      // Stage-2 L1EG efficiency matched WP, for rate comparisons
      bool stage2effMatch_;
      // Crystal pt (in order of strength) for all crystals in the cluster
      std::vector<float> crystalPt_;
      // For investigating novel algorithm parameters
      std::map<std::string, float> experimentalParams_;
  };
  
  
  // Concrete collection of output objects (with extra tuning information)
  typedef std::vector<l1slhc::L1EGCrystalCluster> L1EGCrystalClusterCollection;
}
#endif

