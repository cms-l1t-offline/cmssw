#include "CondFormats/L1TObjects/interface/L1TMuonEndcapParams.h"

void L1TMuonEndcapParams::print(std::ostream& out) const {

  out << "L1 EMTF Parameters" << std::endl;

  out << "Firmware version: " << PtAssignVersion_ << std::endl;

}
