//
// Created by Ole on 16.11.2017.
//

#include <iostream>
#include "automata.h"

void Automata::addStartState(Node *node) {
	startStates.push_back(node);
}

void Automata::addEndState(Node *node) {
	endStates.push_back(node);
}

bool Automata::feed(const char &value) {
	std::vector<Node *> newStates;
	for (Node *node : curStates) {
		std::vector<Node *> nextStates = getNext(value, node);
		for (Node *newNode : nextStates) {
			newStates.push_back(newNode);
		}
	}
	if (newStates.size() == 0)
		return false;
	curStates = newStates;
	return true;
}

std::vector<Node *> Automata::getNext(char condition, Node *current) {
	std::vector<Node *> nextNodes;
	auto it = links.find(current);
	if (it != links.end()) {
		for (Link link : it->second) {
			if (link.condition == condition) {
				nextNodes.push_back(link.next);
			}
		}
	}
	return nextNodes;
}

void Automata::link(char condition, Node *from, Node *to) {
	auto it = links.find(from);
	if (it != links.end()) {
		it->second.emplace_back(Link(condition, to));
	} else {
		links.insert(std::pair<Node *, std::vector<Link>>(from, std::vector<Link>()));
		link(condition, from, to);
	}
}

void Automata::link(char condition, Node *entrance, Node *exit, Automata *automata) {
	for (Node *start : automata->startStates) {
		link(condition, entrance, start);
	}
	for (Node *end : automata->endStates) {
		link(condition, end, exit);
	}
	for(auto link : automata->links) {
		links.insert(link);
	}
}

void Automata::reset() {
	curStates = std::vector<Node *>();
	curStates.reserve(startStates.size());
	for (Node *n : startStates)
		curStates.push_back(n);
}

bool Automata::inEndState() {
	for (Node *node : curStates)
		for (Node *endState : endStates)
			if (node == endState)
				return true;

	return false;
}

void Automata::print() {
	if (inEndState()) {
		std::cout << "Valid end state!" << std::endl;
	} else {
		std::cout << "Invalid end state!" << std::endl;
	}
}
