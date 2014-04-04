
#include "DataFormats/L1Trigger/interface/Jet.h"

l1t::Jet::Jet( const LorentzVector& p4,
	       int pt,
	       int eta,
	       int phi,
	       int qual,
	       bool forward)
  : L1Candidate(p4, pt, eta, phi, qual, 0), forward_(forward)
{
}

l1t::Jet::Jet( const PolarLorentzVector& p4,
	       int pt,
	       int eta,
	       int phi,
	       int qual,
	       bool forward)
  : L1Candidate(p4, pt, eta, phi, qual, 0), forward_(forward)
{
}

l1t::Jet::~Jet()
{

}

bool l1t::Jet::forward() const
{
  return forward_;
}
