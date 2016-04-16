//////////////////////////////////////////////////////////////////////////
//                            EndCapNode.cxx                                  //
// =====================================================================//
// This is the object implementation of a node, which is the            //
// fundamental unit of a decision tree.                                 //                                    
// References include                                                   //
//    *Elements of Statistical Learning by Hastie,                      //
//     Tibshirani, and Friedman.                                        //
//    *Greedy Function Approximation: A Gradient Boosting Machine.      //
//     Friedman. The Annals of Statistics, Vol. 29, No. 5. Oct 2001.    //
//    *Inductive Learning of Tree-based Regression Models. Luis Torgo.  //    
//                                                                      //
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// _______________________Includes_______________________________________//
///////////////////////////////////////////////////////////////////////////

#include "CondFormats/L1TObjects/interface/L1TMuEndCapNode.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include <iostream>
#include <fstream>

//////////////////////////////////////////////////////////////////////////
// _______________________Constructor(s)________________________________//
//////////////////////////////////////////////////////////////////////////

EndCapNode::EndCapNode()
{
    name = "";
    leftDaughter = 0;
    rightDaughter = 0;
    parent = 0;
    splitValue = -99;
    splitVariable = -1;
    avgError = -1;
    totalError = -1;
    errorReduction = -1;
}

EndCapNode::EndCapNode(std::string cName)
{
    name = cName;
    leftDaughter = 0;
    rightDaughter = 0;
    parent = 0;
    splitValue = -99;
    splitVariable = -1;
    avgError = -1;
    totalError = -1;
    errorReduction = -1;
}

//////////////////////////////////////////////////////////////////////////
// _______________________Destructor____________________________________//
//////////////////////////////////////////////////////////////////////////

EndCapNode::~EndCapNode()
{
// Recursively delete all nodes in the tree.
    delete leftDaughter;
    delete rightDaughter;
}

//////////////////////////////////////////////////////////////////////////
// ______________________Get/Set________________________________________//
//////////////////////////////////////////////////////////////////////////

void EndCapNode::setName(std::string sName)
{
    name = sName;
}

std::string EndCapNode::getName()
{
    return name;
}

// ----------------------------------------------------------------------

void EndCapNode::setErrorReduction(Double_t sErrorReduction)
{
    errorReduction = sErrorReduction;
}

Double_t EndCapNode::getErrorReduction()
{
    return errorReduction;
}

// ----------------------------------------------------------------------

void EndCapNode::setLeftDaughter(EndCapNode *sLeftDaughter)
{
    leftDaughter = sLeftDaughter;
}

EndCapNode * EndCapNode::getLeftDaughter()
{
    return leftDaughter;
}

void EndCapNode::setRightDaughter(EndCapNode *sRightDaughter)
{
    rightDaughter = sRightDaughter;
}

EndCapNode * EndCapNode::getRightDaughter()
{
    return rightDaughter;
}

// ----------------------------------------------------------------------

void EndCapNode::setParent(EndCapNode *sParent)
{
    parent = sParent;
}

EndCapNode * EndCapNode::getParent()
{
    return parent;
}

// ----------------------------------------------------------------------

void EndCapNode::setSplitValue(Double_t sSplitValue)
{
    splitValue = sSplitValue;
}

Double_t EndCapNode::getSplitValue()
{
    return splitValue;
}

void EndCapNode::setSplitVariable(Int_t sSplitVar)
{
    splitVariable = sSplitVar;
}

Int_t EndCapNode::getSplitVariable()
{
    return splitVariable;
}

// ----------------------------------------------------------------------

void EndCapNode::setFitValue(Double_t sFitValue)
{
    fitValue = sFitValue;
}

Double_t EndCapNode::getFitValue()
{
    return fitValue;
}

// ----------------------------------------------------------------------

void EndCapNode::setTotalError(Double_t sTotalError)
{
    totalError = sTotalError;
}

Double_t EndCapNode::getTotalError()
{
    return totalError;
}

void EndCapNode::setAvgError(Double_t sAvgError)
{
    avgError = sAvgError;
}

Double_t EndCapNode::getAvgError()
{
    return avgError;
}

// ----------------------------------------------------------------------

void EndCapNode::setNumEvents(Int_t sNumEvents)
{
    numEvents = sNumEvents;
}

Int_t EndCapNode::getNumEvents()
{
    return numEvents;
}

// ----------------------------------------------------------------------

std::vector< std::vector<EndCapEvent*> >& EndCapNode::getEvents()
{
    return events;
}

void EndCapNode::setEvents(std::vector< std::vector<EndCapEvent*> >& sEvents)
{
    events = sEvents;
    numEvents = events[0].size();
}

// ----------------------------------------------------------------------

void EndCapNode::theMiracleOfChildBirth()
{ 
    // Create Daughter Nodes 
    EndCapNode* left = new EndCapNode(name + " left");
    EndCapNode* right = new EndCapNode(name + " right");

    // Link the Nodes Appropriately
    leftDaughter = left;
    rightDaughter = right;
    left->setParent(this);
    right->setParent(this); 
}

// ----------------------------------------------------------------------
EndCapNode* EndCapNode::filterEventToDaughter(EndCapEvent* e)
{
// Anyways, this function takes an event from the parent node
// and filters an event into the left or right daughter
// node depending on whether it is < or > the split point
// for the given split variable. 

    Int_t sv = splitVariable;
    Double_t sp = splitValue;

    EndCapNode* left = leftDaughter;
    EndCapNode* right = rightDaughter;
    EndCapNode* nextNode = 0;

    if(left ==0 || right ==0) return 0;

    if(e->data[sv] < sp) nextNode = left;
    if(e->data[sv] > sp) nextNode = right;
    
    return nextNode;
}
