#include "node.h"
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

using namespace huimacs;
using namespace std;

/* 
 * Initial class static variable
 */
Node*                                  Node::lStartNode     = nullptr;
vector<unsigned>                       Node::twoTWU;
vector<Transaction::oneTWU>*           Node::lOneTWU;
vector<list<pair<unsigned, unsigned>>> Node::indexTable;
const size_t                           Node::sizeIndexTable = 1'024u;


//SMMAS variable
const double Tmax = 1.0;
double Tmin;
double k;
int numberNode;
double checkP;



Node::Node ( Node* iLParentNode, const int& iName, const list<int>& iFollowingNodes, const double& initPheromone ) {
	lParentNode          = iLParentNode;
	name                 = iName;
	followingNodes       = move(iFollowingNodes);
	pheromone            = initPheromone;
	maxCheckPTable       = 0u;
	calculate            = false;

	for (const auto& i : iFollowingNodes) {
		remainNodes.push_back(make_pair(i, (Node*)nullptr));
	}
}


Node::~Node ()
{
	for (auto& i : remainNodes) {
		if (i.second != nullptr)
			delete i.second;
		
	}
}  


Node&
Node::operator = ( const Node &other )
{
	if (this != &other) {
	}
	return *this;
}  


const Node* Node::getStartNode () {
	return Node::lStartNode;
}


int Node::getName () const {
	return name;
}


double Node::getPheromone () const {
	return pheromone;
}

double Node::printPheromone () {
	return checkP;
}


void  Node::positivePrue ( const vector<int> itemset, const PTable& pTable ) {
	if (maxCheckPTable < pTable.maxVersion()) {
		list<list<pair<int, Node*>>::iterator> locationRecord;
		for (auto i = remainNodes.begin(); i != remainNodes.end(); ++i) {
			if (i->second == nullptr) {
				vector<int> tempItemset(itemset);
				tempItemset.push_back(i->first);
				if (pTable.checkTable(tempItemset, maxCheckPTable))
					locationRecord.push_back(i);
			}
		}
		maxCheckPTable = pTable.maxVersion();

		for (auto i : locationRecord)
			remainNodes.erase(i);
	}
}


void Node::localUpdate(const double& rho, const bool isBest){
 	if(isBest){
		pheromone = pheromone * (1.0 - rho) + rho*Tmax;
	}
	else {
		pheromone = pheromone * (1.0 - rho) + rho*Tmin;
	}
}

void Node::globalUpdate ( const vector<int> itemset, const double& rho, const bool isBest) {
	Node* lNode = Node::lStartNode;
	bool find;

	for (auto i : itemset) {
		find = false;
		for (auto j : lNode->remainNodes) {
			Node *lFNode = j.second;
			if (lFNode != nullptr) {
				if (i == lFNode->getName()) {
					find = true;
					lNode = lFNode;
					lNode->addPheromone(rho, isBest);
					break;
				} 
				else 
					break;
			}
		}
		if (!find)
			break;
	}
}
 

Node* Node::initStartNode ( const list<int>& iFollowingNodes, const double& initPheromone ) {
	Node::lStartNode = new Node(nullptr, -1, iFollowingNodes, initPheromone);
	return Node::lStartNode;
}


void Node::deleteStartNode () {
	if (Node::lStartNode != nullptr) delete Node::lStartNode;
}


bool Node::finish () {
	return (remainNodes.size() == 0u);
}


bool Node::calculated () {
	return calculate;
}


void Node::setCalculated () {
	calculate = true;
}


Node* Node::selectNext ( vector<int>& cItemset, const double& alpha, const double& beta, const double& q0, const PTable& pTable, const double& initPheromone ) {
	if (cItemset.size() > 1u)
		positivePrue(cItemset, pTable);

	if (finish()) {
		return this;
	}

	vector<tuple<list<pair<int, Node*>>::iterator, double, double, double>> weightTable;	//double: heuristic, double: pheromone, double: weight
	double pheromone;
	double totalWeight = 0.0;
	double maxWeight = 0.0;
	list<pair<int, Node*>>::iterator iSelectedNode;
	if (cItemset.size() == 0u) {
		for (auto i = remainNodes.begin(); i != remainNodes.end(); ++i) {
			unsigned index = getCandidateIndex(i->first);
			if (i->second != nullptr) {
				pheromone = i->second->pheromone;
			} else {
				pheromone = initPheromone;
			}
			weightTable.push_back(make_tuple(i, (*lOneTWU)[index].twUtility, pheromone, 0.0));
		}
	} else {
		unsigned t1TWU = 0u;
		unsigned t2TWU = 0u;
		list<unsigned> no2TWU;
		for (auto i = remainNodes.begin(); i != remainNodes.end(); ++i) {
			if (i->second != nullptr) {
				pheromone = i->second->pheromone;
			} else {
				pheromone = initPheromone;
			}
			unsigned index1 = getCandidateIndex(name);
			unsigned index2 = getCandidateIndex(i->first);
			unsigned oTWU = (*lOneTWU)[index2].twUtility;
			unsigned tTWU = twoTWU[index1 * (*lOneTWU).size() + index2 - (index1 + 2) * (index1 + 1) / 2];
			if (tTWU != 0u) {
				t1TWU += oTWU;
				t2TWU += tTWU;
				weightTable.push_back(make_tuple(i, tTWU, pheromone, 0.0));
			} else {
				no2TWU.push_back((unsigned)weightTable.size());
				weightTable.push_back(make_tuple(i, oTWU, pheromone, 0.0));
			}
		}
		if (no2TWU.size() != 0u && no2TWU.size() < remainNodes.size()) {
			double a1TWU = (double)t1TWU / (double)(remainNodes.size() - no2TWU.size());
			double a2TWU = (double)t2TWU / (double)(remainNodes.size() - no2TWU.size());
			for (auto& i : no2TWU) {
				get<1>(weightTable[i]) = (get<1>(weightTable[i]) / a1TWU) * a2TWU;
			}
		}
	}

	for (auto& i : weightTable) {
			get<3>(i) = pow(get<1>(i), alpha) * pow(get<2>(i), beta);
			totalWeight += get<3>(i);
			if (get<3>(i) > maxWeight) {
			maxWeight = get<3>(i);
			iSelectedNode = get<0>(i);
        }
	}

	if (iSelectedNode->second == nullptr) {
		list<int> sFollowingNodes;
		auto it = find(followingNodes.begin(), followingNodes.end(), iSelectedNode->first);
		++it;
		for (auto i = it; i != followingNodes.end(); ++i) {
			sFollowingNodes.push_back(*i);
		}
		iSelectedNode->second = new Node(this, iSelectedNode->first, sFollowingNodes, initPheromone);
	}

	cItemset.push_back(iSelectedNode->first);
	return iSelectedNode->second;
}


void  Node::clearRemainNodes () {
	remainNodes.clear();
}


void  Node::setRelatedTransactions ( const list<unsigned> transactions ) {
	relatedTransactions = transactions;
}


list<unsigned> Node::getRelatedTransactions () {
	return relatedTransactions;
}


void Node::initTwoTWU ( const size_t& nCandidate ) {
	//due to positive prunning rule, we can set 0u for pair itemset which is never arrived
	twoTWU.resize((nCandidate * (nCandidate - 1)) / 2, 0u);
	numberNode = twoTWU.size();
	CalTmin();
}


void Node::setLOneTWU ( vector<Transaction::oneTWU>* ILOneTWU ) {
	lOneTWU = ILOneTWU;
}


void Node::setCandidateIndex () {
	indexTable.resize(sizeIndexTable);
	for (size_t i = 0u; i < lOneTWU->size(); ++i) {
		unsigned name = (*lOneTWU)[i].name;
		indexTable[name % sizeIndexTable].push_back(make_pair(name, i));
	}
}


unsigned Node::getCandidateIndex ( const unsigned& name ) {
	unsigned lot = name % sizeIndexTable;
	for (const auto& i : indexTable[lot]) {
		if (i.first == name)
			return i.second;
	}
	return (unsigned)lOneTWU->size();
}


void Node::inputTwoTWU ( const int& name1, const int& name2, const unsigned& utility ) {
	unsigned index1 = getCandidateIndex(name1);
	unsigned index2 = getCandidateIndex(name2);
	twoTWU[index1 * (*lOneTWU).size() + index2 - (index1 + 2) * (index1 + 1) / 2] = utility;
}


void Node::recurisivePrune ( Node* node ) {
	Node* lPreviousNode = node->getLParentNode();
	lPreviousNode->deleteFollowingNode(node);
	delete node;
	if (lPreviousNode->getName() != -1 && lPreviousNode->finish())
		recurisivePrune(lPreviousNode);
}


void Node::deleteFollowingNode ( Node* lNode ) {
	for (auto i = remainNodes.begin(); i != remainNodes.end(); ++i) {
		if (i->first == lNode->getName()) {
			remainNodes.erase(i);
			break;
		}
	}
}


Node* Node::getLParentNode () {
	return lParentNode;
}


 void Node::addPheromone (const double& rho, bool r) {
 	if(r){
		pheromone = pheromone * (1.0 - rho) + rho*Tmax;
	}
	else{
		pheromone = pheromone * (1.0 - rho) + rho*Tmin;
	}
	checkP = pheromone;
 }

void Node::CalTmin(){
	//Calculate Tmax/Tmin
	if(numberNode >=50){
		k = (numberNode + 50) / 100;
	} else {
		k = 1.0;
	}
	
	Tmin = Tmax / (numberNode * k);
	//End Calculate Tmax/Tmin
}

double Node::getTmin(){
	return Tmin;
}

int Node::getNumberNode(){
	return numberNode;
}