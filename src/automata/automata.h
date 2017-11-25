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
public:
	void addStartState(Node *node);
	void addEndState(Node* node);
	bool feed(const std::string& value);
	void doEpsilonMoves();
	std::vector<Node*> getNext(const std::string& condition, Node* current);

	void link(const std::string&condition, Node* from, Node* to);
	void link(const std::string& inCondition, const std::string& outCondition, Node* entrance, Node* exit, Automata* automata);

	void reset();

	bool inEndState();
	void print();
	void printLinks();
};


