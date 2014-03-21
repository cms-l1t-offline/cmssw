
#include "DataFormats/L1Trigger/interface/Tau.h"

l1t::Tau::Tau( const LorentzVector& p4,
	       int pt,
	       int eta,
	       int phi,
	       int qual,
	       int iso )
  : L1Candidate(p4, pt, eta, phi, qual,iso)
{
  if(pt > (1<<numPtBits))
  {
    pt = (1<<numPtBits);
    L1Candidate::setHwPt(pt);
  }
}

l1t::Tau::Tau( const PolarLorentzVector& p4,
	       int pt,
	       int eta,
	       int phi,
	       int qual,
	       int iso )
  : L1Candidate(p4, pt, eta, phi, qual,iso)
{
  if(pt > (1<<numPtBits))
  {
    pt = (1<<numPtBits);
    L1Candidate::setHwPt(pt);
  }
}

void l1t::Tau::setHwPt(int pt)
{
  if(pt > (1<<numPtBits))
    pt = (1<<numPtBits);
  L1Candidate::setHwPt(pt);
}

int l1t::Tau::numPtBits = 30; // don't break old code using signed ints, use huge value

l1t::Tau::~Tau()
{

}
