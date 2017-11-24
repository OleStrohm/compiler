//
// Created by Ole on 16.11.2017.
//

#pragma once
#include "node.h"
#include <vector>
#include <unordered_map>

class Automata {
private:
	std::vector<Node*> endStates;
	std::vector<Node*> startStates;
	std::vector<Node*> curStates;
	std::unordered_map<Node*, std::vector<Link>> links;
public:
	void addStartState(Node *node);
	void addEndState(Node* node);
	bool feed(const char& value);
	std::vector<Node*> getNext(char condition, Node* current);

	void link(char condition, Node* from, Node* to);
	void link(char condition, Node* entrance, Node* exit, Automata* automata);

	void reset();

	bool inEndState();
	void print();
};


