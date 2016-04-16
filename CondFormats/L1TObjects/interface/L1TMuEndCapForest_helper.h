// EndCapForest_helper.h

#ifndef ADD_EndCapFOREST_helper
#define ADD_EndCapFOREST_helper

#include "CondFormats/L1TObjects/interface/L1TMuEndCapForest.h"

class EndCapSubForest : public EndCapForest
{
    public:

        // Constructor(s)/Destructor
        EndCapSubForest();
        ~EndCapSubForest();

        void loadForestFromXML(const char* directory, unsigned int numTrees); 

    private:
		
		COND_SERIALIZABLE;
};

#endif
