//////////////////////////////////////////////////////////////////////////
//                            EndCapTree.cxx                                  //
// =====================================================================//
// This is the object implementation of a decision tree.                //
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

#include "CondFormats/L1TObjects/interface/L1TMuEndCapTree.h"
#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////////
// _______________________Constructor(s)________________________________//
//////////////////////////////////////////////////////////////////////////

EndCapTree::EndCapTree()
{
    rootNode = new EndCapNode("root");

    terminalNodes.push_back(rootNode);
    numTerminalNodes = 1;
}

EndCapTree::EndCapTree(std::vector< std::vector<EndCapEvent*> >& cEvents)
{
    rootNode = new EndCapNode("root");
    rootNode->setEvents(cEvents);

    terminalNodes.push_back(rootNode);
    numTerminalNodes = 1;
}
//////////////////////////////////////////////////////////////////////////
// _______________________Destructor____________________________________//
//////////////////////////////////////////////////////////////////////////


EndCapTree::~EndCapTree()
{
// When the tree is destroyed it will delete all of the nodes in the tree.
// The deletion begins with the rootnode and continues recursively.
    delete rootNode;
}

//////////////////////////////////////////////////////////////////////////
// ______________________Get/Set________________________________________//
//////////////////////////////////////////////////////////////////////////

void EndCapTree::setRootNode(EndCapNode *sRootNode)
{
    rootNode = sRootNode;
}
 
EndCapNode * EndCapTree::getRootNode()
{
     return rootNode;
}

// ----------------------------------------------------------------------

void EndCapTree::setTerminalNodes(std::list<EndCapNode*>& sTNodes)
{
    terminalNodes = sTNodes;
}

std::list<EndCapNode*>& EndCapTree::getTerminalNodes()
{
    return terminalNodes;
}

// ----------------------------------------------------------------------

Int_t EndCapTree::getNumTerminalNodes()
{
    return numTerminalNodes;
}

//////////////////////////////////////////////////////////////////////////
// ______________________Storage/Retrieval______________________________//
//////////////////////////////////////////////////////////////////////////

template <typename T>
std::string numToStr( T num )
{
// Convert a number to a string.
    std::stringstream ss;
    ss << num;
    std::string s = ss.str();
    return  s;
}


// ----------------------------------------------------------------------

void EndCapTree::loadFromXML(const char* filename)
{   
    // First create the engine.
    TXMLEngine* xml = new TXMLEngine;

    // Now try to parse xml file.
    XMLDocPointer_t xmldoc = xml->ParseFile(filename);
    if (xmldoc==0)
    {
        delete xml;
        return;  
    }

    // Get access to main node of the xml file.
    XMLNodePointer_t mainnode = xml->DocGetRootElement(xmldoc);
   
    // Recursively connect nodes together.
    loadFromXMLRecursive(xml, mainnode, rootNode);
   
    // Release memory before exit
    xml->FreeDoc(xmldoc);
    delete xml;
}

// ----------------------------------------------------------------------

void EndCapTree::loadFromXMLRecursive(TXMLEngine* xml, XMLNodePointer_t xnode, EndCapNode* tnode) 
{

    // Get the split information from xml.
    XMLAttrPointer_t attr = xml->GetFirstAttr(xnode);
    std::vector<std::string> splitInfo(3);
    for(unsigned int i=0; i<3; i++)
    {
        splitInfo[i] = xml->GetAttrValue(attr); 
        attr = xml->GetNextAttr(attr);  
    }

    // Convert strings into numbers.
    std::stringstream converter;
    Int_t splitVar;
    Double_t splitVal;
    Double_t fitVal;  

    converter << splitInfo[0];
    converter >> splitVar;
    converter.str("");
    converter.clear();

    converter << splitInfo[1];
    converter >> splitVal;
    converter.str("");
    converter.clear();

    converter << splitInfo[2];
    converter >> fitVal;
    converter.str("");
    converter.clear();

    // Store gathered splitInfo into the node object.
    tnode->setSplitVariable(splitVar);
    tnode->setSplitValue(splitVal);
    tnode->setFitValue(fitVal);

    // Get the xml daughters of the current xml node. 
    XMLNodePointer_t xleft = xml->GetChild(xnode);
    XMLNodePointer_t xright = xml->GetNext(xleft);

    // If there are no daughters we are done.
    if(xleft == 0 || xright == 0) return;

    // If there are daughters link the node objects appropriately.
    tnode->theMiracleOfChildBirth();
    EndCapNode* tleft = tnode->getLeftDaughter();
    EndCapNode* tright = tnode->getRightDaughter();

    // Update the list of terminal nodes.
    terminalNodes.remove(tnode);
    terminalNodes.push_back(tleft);
    terminalNodes.push_back(tright);
    numTerminalNodes++;

    loadFromXMLRecursive(xml, xleft, tleft);
    loadFromXMLRecursive(xml, xright, tright);
}

EndCapNode* EndCapTree::filterEvent(EndCapEvent* e)
{
// Use trees which have already been built to fit a bunch of events
// given by the tEvents vector.

    // Filter the event into a predictive region (terminal node).
    EndCapNode* node = filterEventRecursive(rootNode, e);
    return node;
}

// ----------------------------------------------------------------------

EndCapNode* EndCapTree::filterEventRecursive(EndCapNode* node, EndCapEvent* e)
{
// Filter the event repeatedly into the daughter nodes until it
// falls into a terminal node.


    EndCapNode* nextNode = node->filterEventToDaughter(e);
    if(nextNode == 0) return node;

    return filterEventRecursive(nextNode, e);
}
