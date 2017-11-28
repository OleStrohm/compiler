//
// Created by Ole on 16.11.2017.
//

#pragma once
#include "node.h"
#include <vector>
#include <unordered_map>

#define EPSILON ""

class Automata {
private:
	std::vector<Node*> endStates;
	std::vector<Node*> startStates;
	std::vector<Node*> curStates;
	std::unordered_map<Node*, std::vector<Link>> links;
	std::vector<Node*> nodes;
public:
	std::string id;
	
	Automata(const std::string& id);
	Automata(Automata& original);
	~Automata();
	
	Automata& operator =(Automata& original);

	void addStartState(Node *node);
	void addEndState(Node* node);
	bool feed(const std::string& value);
	void doEpsilonMoves();
	std::vector<Node*> getNext(const std::string& condition, Node* current);

	Node* createNode(const std::string& id);

	void link(const std::string& condition, Node* from, Node* to);
	void link(const std::string& condition, Automata* from, Automata* to);
	void link(const std::string& condition, Automata* from, Node* to);
	void link(const std::string& condition, Node* from, Automata* to);
	void link(const std::string& inCondition, const std::string& outCondition, Node* entrance, Node* exit, Automata* automata);

	void reset();

	bool inEndState();
	void print();
	void printLinks();

	static Automata nullAutomata;
};


