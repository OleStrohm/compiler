//
// Created by Ole on 16.11.2017.
//

#pragma once
#include "node.h"
#include <vector>
#include <unordered_map>

#define EPSILON ""

class Automaton {
private:
	std::vector<Node*> nodes;
	std::unordered_map<Node*, std::vector<Link>> links;
	std::vector<Node*> endStates;
	std::vector<Node*> startStates;
	std::vector<Node*> curStates;
	
	bool failed = true;
public:
	std::string id;
	
	explicit Automaton(const std::string& id);
	Automaton(const Automaton& original);
	Automaton(Automaton&& original) noexcept;
	~Automaton();
	
	Automaton& operator =(const Automaton& original);
	Automaton& operator =(Automaton&& original) noexcept;

	void addStartState(Node *node);
	void addEndState(Node* node);
	bool feed(const std::string& value);
	void doEpsilonMoves();
	std::vector<Node*> getNext(const std::string& condition, Node* current);

	Node* createNode(const std::string& id);

	void link(const std::string& condition, Node* from, Node* to);
	void link(const std::string& condition, Automaton* from, Automaton* to);
	void link(const std::string& condition, Automaton* from, Node* to);
	void link(const std::string& condition, Node* from, Automaton* to);
	void link(const std::string& inCondition, const std::string& outCondition, Node* entrance, Node* exit, Automaton* automata);

	void reset();

	bool inEndState();
	bool hasFailed();
	void print();
	void printLinks();

	static Automaton nullAutomaton;
};


