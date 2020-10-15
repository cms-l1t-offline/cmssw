#include "L1Trigger/L1TMuonEndCap/interface/ShowerRecognition.h"

#include "helper.h"  // to_hex, to_binary

void ShowerRecognition::configure(int verbose,
                                   int endcap,
                                   int sector,
                                   int bx,
                                   int bxWindow,
                                   bool useSecondEarliest) {
  verbose_ = verbose;
  endcap_ = endcap;
  sector_ = sector;
  bx_ = bx;
  bxWindow_ = bxWindow;
  useSecondEarliest_ = useSecondEarliest;

  configure_details();
}

void ShowerRecognition::configure_details() {
  emtf_assert(1 <= bxWindow_ && bxWindow_ <= 3);  // only work for BX windows <= 3
}

void ShowerRecognition::process(const std::deque<EMTFHitCollection>& extended_conv_hits) const {
  // Exit if no hits
  int num_conv_hits = 0;
  for (const auto& conv_hits : extended_conv_hits)
    num_conv_hits += conv_hits.size();
  bool early_exit = (num_conv_hits == 0);

  if (early_exit)
    return;

  // debug
  if (verbose_ > 0) {
    for (const auto& conv_hits : extended_conv_hits) {
      for (const auto& conv_hit : conv_hits) {
        if (conv_hit.Subsystem() == TriggerPrimitive::kCSC) {
          std::cout << "CSC hit st: " << conv_hit.Station()
                    << " ri: " << conv_hit.Ring()
                    << " ch: " << conv_hit.Chamber()
                    << " bx: " << conv_hit.BX()
                    << std::endl;
        }
      }
    }
  }  // end debug
  // shower recognition goes here
}
