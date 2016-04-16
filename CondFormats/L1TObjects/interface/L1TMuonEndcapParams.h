///
/// \class L1TMuonEndcapParams
///
/// Description: Placeholder for EMTF parameters
///
///
/// \author: Matthew Carver
///

#ifndef L1TEMTFParams_h
#define L1TEMTFParams_h

#include <memory>
#include <iostream>
#include <vector>

#include "CondFormats/Serialization/interface/Serializable.h"
#include "CondFormats/L1TObjects/interface/LUT.h"
#include "CondFormats/L1TObjects/interface/L1TMuEndCapForest_helper.h"

class L1TMuonEndcapParams {

public:

	void SetPtAssignVersion(unsigned version){PtAssignVersion_ = version;};
	void SetSt1PhiMatchWindow(int window){PhiMatchWindowSt1_ = window;};
	void SetSt2PhiMatchWindow(int window){PhiMatchWindowSt2_ = window;};
	void SetSt3PhiMatchWindow(int window){PhiMatchWindowSt3_ = window;};
	void SetSt4PhiMatchWindow(int window){PhiMatchWindowSt4_ = window;};
	void SetPtForests(std::vector<std::pair<int,EndCapSubForest*>> forests){Forests_ = forests;};
	
	unsigned GetPtAssignVersion(){return PtAssignVersion_;};
	int GetSt1PhiMatchWindow() const {return PhiMatchWindowSt1_;};
	int GetSt2PhiMatchWindow() const {return PhiMatchWindowSt2_;};
	int GetSt3PhiMatchWindow() const {return PhiMatchWindowSt3_;};
	int GetSt4PhiMatchWindow() const {return PhiMatchWindowSt4_;};
	std::vector<std::pair<int,EndCapSubForest*>> getPtForests() const {return Forests_;};
	
	
	L1TMuonEndcapParams() { PtAssignVersion_=1; }
   ~L1TMuonEndcapParams() {}

	void print(std::ostream&) const;

private:
	
  unsigned PtAssignVersion_;
  int PhiMatchWindowSt1_, PhiMatchWindowSt2_, PhiMatchWindowSt3_, PhiMatchWindowSt4_;
  std::vector<std::pair<int,EndCapSubForest*>> Forests_;

  COND_SERIALIZABLE;
};
#endif
