//////////////////////////////////////////////////////////////////////////
//                            EndCapForest.cxx                                //
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

#include "CondFormats/L1TObjects/interface/L1TMuEndCapForest.h"

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

EndCapForest::EndCapForest()
{
    events = std::vector< std::vector<EndCapEvent*> >(1);
}

EndCapForest::~EndCapForest()
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

// return the ith tree
EndCapTree* EndCapForest::getTree(unsigned int i)
{ 
    if(/*i>=0 && */i<trees.size()) return trees[i]; 
    else
    {
        std::cout << i << "is an invalid input for getTree. Out of range." << std::endl;
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////////
// ______________________Various_Helpful_Functions______________________//
//////////////////////////////////////////////////////////////////////////

unsigned int EndCapForest::size()
{
// Return the number of trees in the forest.
    return trees.size();
}


void EndCapForest::loadForestFromXML(const char* directory, unsigned int numTrees)
{
// Load a forest that has already been created and stored into XML somewhere.

    // Initialize the vector of trees.
    trees = std::vector<EndCapTree*>(numTrees);

    // Load the EndCapForest.
    //std::cout << std::endl << "Loading EndCapForest from XML ... " << std::endl;
    for(unsigned int i=0; i < numTrees; i++) 
    {   
        trees[i] = new EndCapTree(); 

        std::stringstream ss;
        ss << directory << "/" << i << ".xml";

        //trees[i]->loadFromXML(ss.str().c_str());
		trees[i]->loadFromXML(edm::FileInPath(ss.str().c_str()).fullPath().c_str());
    }   

   // std::cout << "Done." << std::endl << std::endl;
}

void EndCapForest::predictEvent(EndCapEvent* e, unsigned int numtrees)
{
// Predict values for eventsp by running them through the forest up to numtrees.

    //std::cout << "Using " << numtrees << " trees from the forest to predict events ... " << std::endl;
    if(numtrees > trees.size())
    {
        std::cout << std::endl << "!! Input greater than the forest size. Using forest.size() = " << trees.size() << " to predict instead." << std::endl;
        numtrees = trees.size();
    }

    // i iterates through the trees in the forest. Each tree corrects the last prediction.
    for(unsigned int i=0; i < numtrees; i++) 
    {
        //std::cout << "++Tree " << i << "..." << std::endl;
        appendCorrection(e, i);
    }
}

//////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////

void EndCapForest::appendCorrection(EndCapEvent* e, Int_t treenum)
{
// Update the prediction by appending the next correction.

    EndCapTree* tree = trees[treenum];
    EndCapNode* terminalNode = tree->filterEvent(e); 

    // Update the event with its new prediction.
    Double_t fit = terminalNode->getFitValue();
    e->predictedValue += fit;
}
