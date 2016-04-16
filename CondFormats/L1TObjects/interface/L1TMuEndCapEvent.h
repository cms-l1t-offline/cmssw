//////////////////////////////////////////////////////////////////////////
// ---------------------------EndCapEvent.h------------------------------------
//////////////////////////////////////////////////////////////////////////

#ifndef ADD_EndCapEVENT
#define ADD_EndCapEVENT

#include "TMath.h"
#include <vector>
#include <iostream>
#include "CondFormats/Serialization/interface/Serializable.h"

//////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////

struct EndCapEvent
{

    Double_t trueValue;
    Double_t predictedValue;
    Double_t DTPt;
    Double_t CSCPt;
    Double_t tmvaPt;
    Double_t tmvaPt1;
    Int_t Mode;
    Int_t Quality;

    static Int_t sortingIndex;
    Int_t id;    
    std::vector<Double_t> data;         

    bool operator< (const EndCapEvent &rhs) const
    {
        return data[sortingIndex] < rhs.data[sortingIndex];
    }
	
	COND_SERIALIZABLE;
};

#endif
