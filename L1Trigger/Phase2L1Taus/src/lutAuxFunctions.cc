#include "L1Trigger/Phase2L1Taus/interface/lutAuxFunctions.h"

#include "FWCore/Utilities/interface/Exception.h"

#include <TFile.h> // TFile
#include <TH1.h>   // TH1
#include <TH2.h>   // TH2

/**
 * @brief Open ROOT file the name of which is given as function argument
 * @param fileName: name of ROOT file
 * @return pointer to TFile object
 */
TFile *
openFile(const LocalFileInPath & fileName)
{
  if(fileName.fullPath().empty())
  {
    throw cms::Exception("openFile") 
      << " Failed to find file = " << fileName << " !!\n";
  }
  TFile * inputFile = new TFile(fileName.fullPath().data());
  return inputFile;
}

/**
 * @brief Load one-dimensional histogram (TH1) from ROOT file 
 * @param fileName:      name of ROOT file
 * @param histogramName: name of the histogram
 * @return pointer to TH1 object
 */
TH1 *
loadTH1(TFile * inputFile,
        const std::string & histogramName)
{
  TH1* histogram = dynamic_cast<TH1 *>(inputFile->Get(histogramName.data()));
  if (! histogram)
  {
    throw cms::Exception("loadTH1") 
      << " Failed to load TH1 = " << histogramName.data() << " from file = " << inputFile->GetName() << " !!\n";;
  }
  return histogram;
}

/**
 * @brief Load two-dimensional histogram (TH2) from ROOT file 
 * @param fileName:      name of ROOT file
 * @param histogramName: name of the histogram
 * @return pointer to TH2 object
 */
TH2 *
loadTH2(TFile * inputFile,
        const std::string & histogramName)
{
  TH2 * histogram = dynamic_cast<TH2 *>(inputFile->Get(histogramName.data()));
  if(! histogram)
  {
    throw cms::Exception("loadTH2") 
      << " Failed to load TH2 = " << histogramName.data() << " from file = " << inputFile->GetName() << " !!\n";
  }
  return histogram;
}
