#ifndef DQM_SiStripCommissioningClients_PedestalsHistograms_H
#define DQM_SiStripCommissioningClients_PedestalsHistograms_H

#include "DQM/SiStripCommissioningClients/interface/CommissioningHistograms.h"

class MonitorUserInterface;

class PedestalsHistograms : public CommissioningHistograms {

 public:
  
  /** */
  PedestalsHistograms( MonitorUserInterface* );
  /** */
  virtual ~PedestalsHistograms();

 private: // ----- private methods -----
  
  virtual void book( const std::vector<std::string>& me_list );
  virtual void update();
  
  //void constructHistoSets( const vector<string>& me_list );
  
 private: // ----- private data members -----

  // One map entry per LLD channel...
  std::map< uint32_t, HistoSet > mePeds_;
  std::map< uint32_t, HistoSet > meNoise_;
  std::map< uint32_t, HistoSet > meCommonModeApv0_;
  std::map< uint32_t, HistoSet > meCommonModeApv1_;
  
};

#endif // DQM_SiStripCommissioningClients_PedestalsHistograms_H


