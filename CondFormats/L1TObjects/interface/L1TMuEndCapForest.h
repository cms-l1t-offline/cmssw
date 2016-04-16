// EndCapForest.h

#ifndef ADD_EndCapFOREST
#define ADD_EndCapFOREST

#include "CondFormats/L1TObjects/interface/L1TMuEndCapTree.h"

class EndCapForest
{
    public:

        // Constructor(s)/Destructor
        EndCapForest();
        ~EndCapForest();

        // Returns the number of trees in the forest.
        unsigned int size();
		
        void loadForestFromXML(const char* directory, unsigned int numTrees); 
		void appendCorrection(EndCapEvent* e, Int_t treenum);
        void predictEvent(EndCapEvent* e, unsigned int trees);


        EndCapTree* getTree(unsigned int i);

    protected:

        std::vector< std::vector<EndCapEvent*> > events;
        std::vector< std::vector<EndCapEvent*> > subSample;
        std::vector<EndCapTree*> trees;
		
		COND_SERIALIZABLE;
};

#endif
