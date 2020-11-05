#ifndef L1Trigger_L1TMuonEndCap_ShowerRecognition_h
#define L1Trigger_L1TMuonEndCap_ShowerRecognition_h

#include "L1Trigger/L1TMuonEndCap/interface/Common.h"

class ShowerRecognition {
public:
  void configure(int verbose,
                 int endcap,
                 int sector,
                 int bx,
                 int bxWindow,
                 bool useSecondEarliest);

  void configure_details();

  void process(const std::deque<EMTFHitCollection>& extended_conv_hits,
               RegionalMuonShowerBxCollection& best_showers) const;

private:
  int verbose_, endcap_, sector_, bx_;
  int bxWindow_;
  bool useSecondEarliest_;
};

#endif
