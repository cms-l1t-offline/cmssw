// L1TMuEndCapNode.h

#ifndef ADD_EndCapNODE
#define ADD_EndCapNODE

#include <string>
#include <vector>
#include "CondFormats/L1TObjects/interface/L1TMuEndCapEvent.h"
#include "CondFormats/Serialization/interface/Serializable.h"

class EndCapNode
{
    public:
        EndCapNode();
        EndCapNode(std::string cName);
        ~EndCapNode();

        std::string getName();
        void setName(std::string sName);

        Double_t getErrorReduction();
        void setErrorReduction(Double_t sErrorReduction);

        EndCapNode * getLeftDaughter();
        void setLeftDaughter(EndCapNode *sLeftDaughter);

        EndCapNode * getRightDaughter();
        void setRightDaughter(EndCapNode *sLeftDaughter);

        EndCapNode * getParent();
        void setParent(EndCapNode *sParent);

        Double_t getSplitValue();
        void setSplitValue(Double_t sSplitValue);

        Int_t getSplitVariable();
        void setSplitVariable(Int_t sSplitVar);

        Double_t getFitValue();
        void setFitValue(Double_t sFitValue);

        Double_t getTotalError();
        void setTotalError(Double_t sTotalError);

        Double_t getAvgError();
        void setAvgError(Double_t sAvgError);

        Int_t getNumEvents();
        void setNumEvents(Int_t sNumEvents);

        std::vector< std::vector<EndCapEvent*> >& getEvents();
        void setEvents(std::vector< std::vector<EndCapEvent*> >& sEvents);

        void theMiracleOfChildBirth();
		
		EndCapNode* filterEventToDaughter(EndCapEvent* e);
 
    private:
	std::string name;

        EndCapNode *leftDaughter;
        EndCapNode *rightDaughter;
        EndCapNode *parent;

        Double_t splitValue;
        Int_t splitVariable;

        Double_t errorReduction;
        Double_t totalError;
        Double_t avgError;

        Double_t fitValue;
        Int_t numEvents;

        std::vector< std::vector<EndCapEvent*> > events;
		
		COND_SERIALIZABLE;
};

#endif
