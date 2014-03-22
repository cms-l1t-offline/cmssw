
#include "DataFormats/L1Trigger/interface/Jet.h"

l1t::Jet::Jet( const LorentzVector& p4,
	       int pt,
	       int eta,
	       int phi,
	       int qual )
  : L1Candidate(p4, pt, eta, phi, qual, 0)
{
  if(pt > (1<<numPtBits))
  {
    pt = (1<<numPtBits);
    L1Candidate::setHwPt(pt);
  }
}

l1t::Jet::Jet( const PolarLorentzVector& p4,
	       int pt,
	       int eta,
	       int phi,
	       int qual )
  : L1Candidate(p4, pt, eta, phi, qual, 0)
{
  if(pt > (1<<numPtBits))
  {
    pt = (1<<numPtBits);
    L1Candidate::setHwPt(pt);
  }
}

void l1t::Jet::setHwPt(int pt)
{
  if(pt > (1<<numPtBits))
    pt = (1<<numPtBits);
  L1Candidate::setHwPt(pt);
}

int l1t::Jet::numPtBits = 30; // don't break old code using signed ints, use huge value

l1t::Jet::~Jet()
{

}
