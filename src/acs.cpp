#include "acs.h"

using namespace huimacs;
using namespace std;


ACS::ACS ( const unsigned& iSize, Transaction* iLDataset, Node* iLSNode, double iInitialPheromone, double iRho, const double& iAlpha, const double& iBeta, const double& iQ0 ) {
	nIteration     = 1u;
	populationSize = iSize;
	lDataset       = iLDataset;
	lSNode         = iLSNode;
	initPheromone  = iInitialPheromone;
	rho            = iRho;
	alpha          = iAlpha;
	beta           = iBeta;
	q0             = iQ0;
}


ACS::~ACS ()
{
}  


ACS&
ACS::operator = ( const ACS &other )
{
	if ( this != &other ) {
	}
	return *this;
}  



unsigned ACS::getNIteration () {
	return nIteration;
}


unsigned ACS::getLastFind () {
	return lastFind;
}


void ACS::runIteration () {
	Node* lCNode;
	vector<int>    cItemset;
	vector<int>*   lBestItemset = nullptr;
	unsigned       bestUtility  = 0u;

	list<unsigned> relatedTransactions;
	bool isBest;

	for (unsigned i = 0u; i < populationSize; ++i) {
		lCNode = lSNode;
		cItemset.clear();
		relatedTransactions.clear();
		while (!lCNode->finish()) {
			lCNode = lCNode->selectNext(cItemset, alpha, beta, q0, pTable, initPheromone);

			if (!lCNode->calculated()) {
				auto result = lDataset->calculateUtility(relatedTransactions, cItemset);
				lCNode->setRelatedTransactions(relatedTransactions);
				if (get<1>(result)) {
					huis.push_back(make_pair(cItemset, get<0>(result)));
					if (bestUtility < get<0>(result)) {
						bestUtility = get<0>(result);
						if (lBestItemset != nullptr){
						 	delete lBestItemset;
						} 
						lBestItemset = new vector<int>(cItemset);
					} 
				} else if (!get<2>(result)) {					
					pTable.insertRecord(cItemset);
					lCNode->clearRemainNodes();
				} else {
					lCNode->localUpdate(rho, isBest);
				}
				if (cItemset.size() == 2u) Node::inputTwoTWU(cItemset.front(), cItemset.back(), get<3>(result));
				lCNode->setCalculated();
			} else {
				relatedTransactions = lCNode->getRelatedTransactions();
			}
			
		}

		if (lCNode->getName() == -1) break;
		Node::recurisivePrune(lCNode);
	}
	if (lBestItemset != nullptr) {
		Node::globalUpdate (*lBestItemset, rho, true);
		delete lBestItemset;
		lastFind = nIteration;
	} 
	++nIteration;
}
