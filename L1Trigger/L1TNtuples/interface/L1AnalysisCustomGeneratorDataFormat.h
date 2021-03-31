#ifndef __L1Analysis_L1AnalysisCustomGeneratorDataFormat_H__
#define __L1Analysis_L1AnalysisCustomGeneratorDataFormat_H__

//-------------------------------------------------------------------------------
// Created 15/04/2010 - E. Conte, A.C. Le Bihan
// Edited  18/06/2018 - M. Toumazou
//
// Original code : L1Trigger/L1TNtuples/L1NtupleProducer
//-------------------------------------------------------------------------------
#include <TROOT.h>
#include <vector>
//#include <TString.h>

namespace L1Analysis {
  struct L1AnalysisCustomGeneratorDataFormat {
    L1AnalysisCustomGeneratorDataFormat() { Reset(); };
    ~L1AnalysisCustomGeneratorDataFormat(){};

    void Reset() {
      weight = -999.;
      pthat = -999.;
      nVtx = 0;
      nMeanPU = 0;

      nPart = 0;
      partId.resize(0);
      partStat.resize(0);
      partPt.resize(0);
      partEta.resize(0);
      partPhi.resize(0);
      partMass.resize(0);
      partE.resize(0);
      partCh.resize(0);
      partVertexX.resize(0);
      partVertexY.resize(0);
      partVertexZ.resize(0);
      partMothers.resize(0);
      partDaughters.resize(0);

      nJet = 0;
      jetPt.resize(0);
      jetEta.resize(0);
      jetPhi.resize(0);
      jetM.resize(0);
    }

    // ---- L1AnalysisCustomGeneratorDataFormat information.

    float weight;
    float pthat;
    int nVtx;
    int nMeanPU;

    int nPart;
    std::vector<int> partId;
    std::vector<int> partStat;
    std::vector<float> partPt;
    std::vector<float> partEta;
    std::vector<float> partPhi;
    std::vector<float> partMass;
    std::vector<float> partE;
    std::vector<int> partCh;
    std::vector<float> partVertexX;
    std::vector<float> partVertexY;
    std::vector<float> partVertexZ;
    std::vector<std::vector<unsigned short> > partMothers;
    std::vector<std::vector<unsigned short> > partDaughters;

    int nJet;
    std::vector<float> jetPt;
    std::vector<float> jetEta;
    std::vector<float> jetPhi;
    std::vector<float> jetM;
  };
}  // namespace L1Analysis
#endif
