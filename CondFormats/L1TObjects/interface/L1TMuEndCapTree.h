// EndCapTree.h

#ifndef ADD_EndCapTREE
#define ADD_EndCapTREE

#include <list>
#include "CondFormats/L1TObjects/interface/L1TMuEndCapNode.h"
#include "TXMLEngine.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/Serialization/interface/Serializable.h"



class EndCapTree
{
    public:
        EndCapTree();
        EndCapTree(std::vector< std::vector<EndCapEvent*> >& cEvents);
        ~EndCapTree();

        void setRootNode(EndCapNode *sRootNode);
        EndCapNode* getRootNode();

        void setTerminalNodes(std::list<EndCapNode*>& sTNodes);
        std::list<EndCapNode*>& getTerminalNodes();

        Int_t getNumTerminalNodes();

        void addXMLAttributes(TXMLEngine* xml, EndCapNode* node, XMLNodePointer_t np);

        void loadFromXML(const char* filename);
        void loadFromXMLRecursive(TXMLEngine* xml, XMLNodePointer_t node, EndCapNode* tnode);
		
		EndCapNode* filterEvent(EndCapEvent* e);
        EndCapNode* filterEventRecursive(EndCapNode* node, EndCapEvent* e);

    private:
        EndCapNode *rootNode;
        std::list<EndCapNode*> terminalNodes;
        Int_t numTerminalNodes;
        Double_t rmsError;
		
		COND_SERIALIZABLE;
};

#endif
