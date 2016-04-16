//////////////////////////////////////////////////////////////////////////
//                            EndCapSubForest.cxx                                //
// =====================================================================//
// This is the object implementation of a forest of decision trees.     //
// We need this to implement gradient boosting.                         //
// References include                                                   //
//    *Elements of Statistical Learning by Hastie,                      //
//     Tibshirani, and Friedman.                                        //
//    *Greedy Function Approximation: A Gradient Boosting Machine.      //
//     Friedman. The Annals of Statistics, Vol. 29, No. 5. Oct 2001.    //
//    *Inductive Learning of EndCapTree-based Regression Models. Luis Torgo.  //    
//                                                                      //
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// _______________________Includes_______________________________________//
///////////////////////////////////////////////////////////////////////////

#include "CondFormats/L1TObjects/interface/L1TMuEndCapForest_helper.h"

#include "TRandom3.h"
#include "TStopwatch.h"
#include "TROOT.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TChain.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <utility>

//////////////////////////////////////////////////////////////////////////
// _______________________Constructor(s)________________________________//
//////////////////////////////////////////////////////////////////////////

EndCapSubForest::EndCapSubForest()
{
    events = std::vector< std::vector<EndCapEvent*> >(1);
}

EndCapSubForest::~EndCapSubForest()
{
// When the forest is destroyed it will delete the trees as well as the
// events from the training and testing sets.
// The user may want the events to remain after they destroy the forest
// this should be changed in future upgrades.

    for(unsigned int i=0; i < trees.size(); i++)
    { 
        delete trees[i];
    }
}


void EndCapSubForest::loadForestFromXML(const char* directory, unsigned int numTrees)
{
// Load a forest that has already been created and stored into XML somewhere.

    // Initialize the vector of trees.
    trees = std::vector<EndCapTree*>(numTrees);

    // Load the EndCapSubForest.
    for(unsigned int i=0; i < numTrees; i++) 
    {   
        trees[i] = new EndCapTree(); 

        std::stringstream ss;
        ss << directory << "/" << i << ".xml";

		trees[i]->loadFromXML(edm::FileInPath(ss.str().c_str()).fullPath().c_str());
    }   

}
