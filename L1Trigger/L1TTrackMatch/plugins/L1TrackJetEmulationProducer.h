#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstdlib>
#include "DataFormats/L1Trigger/interface/TkJetWord.h"

using namespace std;

//Each individual box in the eta and phi dimension.
//  Also used to store final cluster data for each zbin.
struct EtaPhiBin {
  l1t::pt_t pTtot;
  l1t::nt_t ntracks;
  l1t::nx_t nxtracks;
  bool used;
  l1t::glbphi_t phi;  //average phi value (halfway b/t min and max)
  l1t::glbeta_t eta;  //average eta value
};

//store important information for plots
struct MaxZBin {
  int znum;    //Numbered from 0 to nzbins (16, 32, or 64) in order
  int nclust;  //number of clusters in this bin
  l1t::z0_t zbincenter;
  EtaPhiBin *clusters;  //list of all the clusters in this bin
  l1t::pt_t ht;         //sum of all cluster pTs--only the zbin with the maximum ht is stored
};
