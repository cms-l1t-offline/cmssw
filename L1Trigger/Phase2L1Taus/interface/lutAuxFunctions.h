#ifndef L1Trigger_Phase2L1Taus_lutAuxFunctions_h
#define L1Trigger_Phase2L1Taus_lutAuxFunctions_h

#include <FWCore/ParameterSet/interface/ParameterSet.h> // edm::ParameterSet

#include "L1Trigger/Phase2L1Taus/interface/LocalFileInPath.h" // LocalFileInPath

// forward declarations
class TFile;
class TH1;
class TH2;

// define auxiliary functions
TFile *
openFile(const LocalFileInPath & fileName);

TH1 *
loadTH1(TFile * inputFile,
        const std::string & histogramName);

TH2 *
loadTH2(TFile * inputFile,
        const std::string & histogramName);

#endif // L1Trigger_Phase2L1Taus_lutAuxFunctions_h
