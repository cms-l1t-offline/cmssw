#include "L1Trigger/L1TMuonEndCap/interface/SectorProcessorShower.h"
#define NUM_CSC_CHAMBERS 4 * 9

SectorProcessorShower::SectorProcessorShower() {}

SectorProcessorShower::~SectorProcessorShower() {}

void SectorProcessorShower::configure(const edm::ParameterSet& pset, int verbose, int endcap, int sector) {
  emtf_assert(emtf::MIN_ENDCAP <= endcap && endcap <= emtf::MAX_ENDCAP);
  emtf_assert(emtf::MIN_TRIGSECTOR <= sector && sector <= emtf::MAX_TRIGSECTOR);

  verbose_ = verbose;
  endcap_ = endcap;
  sector_ = sector;

  auto spShowerParams = pset.getParameter<edm::ParameterSet>("spShowerParams");
  enableTwoLoose_ = spShowerParams.getParameter<bool>("enableTwoLoose");
  enableOneNominal_ = spShowerParams.getParameter<bool>("enableOneNominal");
}

void SectorProcessorShower::process(const CSCShowerDigiCollection& in_showers,
                                    RegionalMuonShowerBxCollection& out_showers) const {
  // reset
  std::vector<CSCShowerDigi> selected_showers;

  // shower selection
  auto chamber = in_showers.begin();
  auto chend = in_showers.end();
  for (; chamber != chend; ++chamber) {
    auto digi = (*chamber).second.first;
    auto dend = (*chamber).second.second;
    for (; digi != dend; ++digi) {
      // Returns CSC "link" index (0 - 45)
      int selected_shower = select_shower((*chamber).first, *digi);

      // index is valid
      if (selected_shower >= 0) {
        emtf_assert(selected_shower < NUM_CSC_CHAMBERS);

        // shower is valid
        if (digi->isValid()) {
          selected_showers.emplace_back(*digi);
        }
      }
    }
  }

  for (const auto& shower : selected_showers) {
    std::cout << "SectorProcessorShower :: Found CSCShowerDigi in endcap " << endcap_ << " sector " << sector_
              << " --> bits " << shower.bits() << std::endl;
  }

  // Shower recognition logic: at least one nominal shower (see DN-20-033, section 5.2)
  const unsigned nLooseInTime(std::count_if(
      selected_showers.begin(), selected_showers.end(), [](CSCShowerDigi p) { return p.isLooseInTime(); }));
  const unsigned nNominalInTime(std::count_if(
      selected_showers.begin(), selected_showers.end(), [](CSCShowerDigi p) { return p.isNominalInTime(); }));
  const unsigned nLooseOutTime(std::count_if(
      selected_showers.begin(), selected_showers.end(), [](CSCShowerDigi p) { return p.isLooseOutTime(); }));
  const unsigned nNominalOutTime(std::count_if(
      selected_showers.begin(), selected_showers.end(), [](CSCShowerDigi p) { return p.isNominalOutTime(); }));

  const bool hasTwoLooseInTime(nLooseInTime >= 2);
  const bool hasOneNominalInTime(nNominalInTime >= 1);
  const bool hasTwoLooseOutTime(nLooseOutTime >= 2);
  const bool hasOneNominalOutTime(nNominalOutTime >= 1);

  const bool acceptLoose(enableTwoLoose_ and (hasTwoLooseInTime or hasTwoLooseOutTime));
  const bool acceptNominal(enableOneNominal_ and (hasOneNominalInTime or hasOneNominalOutTime));
  // trigger condition
  const bool accept(acceptLoose or acceptNominal);

  if (accept) {
    // shower output
    l1t::RegionalMuonShower out_shower;
    out_shower.setEndcap(endcap_);
    out_shower.setSector(sector_);
    if (hasOneNominalInTime)
      out_shower.setBitsInTime(l1t::RegionalMuonShower::kOneNominal);
    if (hasTwoLooseInTime)
      out_shower.setBitsInTime(l1t::RegionalMuonShower::kTwoLoose);
    if (hasOneNominalOutTime)
      out_shower.setBitsOutTime(l1t::RegionalMuonShower::kOneNominal);
    if (hasTwoLooseOutTime)
      out_shower.setBitsOutTime(l1t::RegionalMuonShower::kTwoLoose);
    out_showers.push_back(0, out_shower);
  }
}

// shower selection
int SectorProcessorShower::select_shower(const CSCDetId& tp_detId, const CSCShowerDigi& shower) const {
  int selected = -1;

  int tp_endcap = tp_detId.endcap();
  int tp_sector = tp_detId.triggerSector();
  int tp_station = tp_detId.station();
  int tp_chamber = tp_detId.chamber();
  int tp_csc_ID = shower.getCSCID();

  // station 1 --> subsector 1 or 2
  // station 2,3,4 --> subsector 0
  int tp_subsector = (tp_station != 1) ? 0 : ((tp_chamber % 6 > 2) ? 1 : 2);

  // Check if the chamber belongs to this sector processor at this BX.
  selected = get_index_shower(tp_endcap, tp_sector, tp_subsector, tp_station, tp_csc_ID);
  return selected;
}

int SectorProcessorShower::get_index_shower(
    int tp_endcap, int tp_sector, int tp_subsector, int tp_station, int tp_csc_ID) const {
  int selected = -1;

  // shower trigger does not considers overlaps
  if (is_in_sector_csc(tp_endcap, tp_sector)) {
    if (tp_station == 1) {  // ME1: 0 - 8, 9 - 17
      selected = (tp_subsector - 1) * 9 + (tp_csc_ID - 1);
    } else {  // ME2,3,4: 18 - 26, 27 - 35, 36 - 44
      selected = (tp_station)*9 + (tp_csc_ID - 1);
    }
  }

  emtf_assert(selected != -1);
  return selected;
}

bool SectorProcessorShower::is_in_sector_csc(int tp_endcap, int tp_sector) const {
  return ((endcap_ == tp_endcap) && (sector_ == tp_sector));
}
