#include "DataFormats/L1Trigger/interface/EtSum.h"

l1t::EtSum::EtSum( const LorentzVector& p4,
		   EtSumType type,
		   int pt,
		   int eta,
		   int phi,
		   int qual)
  : L1Candidate(p4, pt, eta, phi, qual, 0),
      type_(type)
{
  if(pt > (1<<numPtBits))
  {
    pt = (1<<numPtBits);
    L1Candidate::setHwPt(pt);
  }
}

l1t::EtSum::EtSum( const PolarLorentzVector& p4,
		   EtSumType type,
		   int pt,
		   int eta,
		   int phi,
		   int qual)
  : L1Candidate(p4, pt, eta, phi, qual, 0),
      type_(type)
{
  if(pt > (1<<numPtBits))
  {
    pt = (1<<numPtBits);
    L1Candidate::setHwPt(pt);
  }
}

void l1t::EtSum::setHwPt(int pt)
{
  if(pt > (1<<numPtBits))
    pt = (1<<numPtBits);
  L1Candidate::setHwPt(pt);
}

int l1t::EtSum::numPtBits = 30; // don't break old code using signed ints, use huge value

l1t::EtSum::~EtSum()
{

}

void l1t::EtSum::setType(EtSumType type)
{
  type_ = type;
}

l1t::EtSum::EtSumType l1t::EtSum::getType() const
{
  return type_;
}
