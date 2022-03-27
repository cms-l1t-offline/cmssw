#ifndef DataFormatsL1TCorrelator_TPJetFwd_h
#define DataFormatsL1TCorrelator_TPJetFwd_h

// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     TPJetFwd
//

#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"

namespace l1t {

  class TPJet;

  typedef edm::RefProd<TPJet> TPJetRefProd;

  typedef std::vector<TPJet> TPJetCollection;

  typedef edm::Ref<TPJetCollection> TPJetRef;
  typedef edm::RefVector<TPJetCollection> TPJetRefVector;
  typedef std::vector<TPJetRef> TPJetVectorRef;
}  // namespace l1t

#endif
