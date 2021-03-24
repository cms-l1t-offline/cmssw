#ifndef L1Trigger_L1TMuonEndCap_SectorProcessorShower_h
#define L1Trigger_L1TMuonEndCap_SectorProcessorShower_h

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "FWCore/Framework/interface/Event.h"
#include "L1Trigger/L1TMuonEndCap/interface/Common.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class SectorProcessorShower {
public:
  explicit SectorProcessorShower();
  ~SectorProcessorShower();

  void configure(const edm::ParameterSet&, int verbose, int endcap, int sector);

  void process(const CSCShowerDigiCollection& showers, RegionalMuonShowerBxCollection& out_showers) const;

private:
  int select_shower(const CSCDetId&, const CSCShowerDigi&) const;
  int get_index_shower(int tp_endcap, int tp_sector, int tp_subsector, int tp_station, int tp_csc_ID) const;
  bool is_in_sector_csc(int tp_endcap, int tp_sector) const;

  int verbose_, endcap_, sector_;
  bool enableTwoLoose_;
  bool enableOneNominal_;
};

#endif
