//
// Created by Ole on 16.11.2017.
//

#include <iostream>
#include "automaton.h"

Automaton Automaton::nullAutomaton("null");

Automaton::Automaton(const std::string& id)
		: id(id) {}

Automaton::Automaton(const Automaton& original)
		: id(original.id) {}

Automaton::Automaton(Automaton&& original) noexcept {
	std::swap(id, original.id);
	std::swap(nodes, original.nodes);
	std::swap(links, original.links);
	std::swap(startStates, original.startStates);
	std::swap(endStates, original.endStates);
}

Automaton::~Automaton() {
	for (Node* node : nodes)
		delete node;
}

Automaton& Automaton::operator=(const Automaton& original) {
	this->id = original.id;
	return *this;
}

Automaton& Automaton::operator =(Automaton&& original) noexcept {
	std::swap(id, original.id);
	std::swap(nodes, original.nodes);
	std::swap(links, original.links);
	std::swap(startStates, original.startStates);
	std::swap(endStates, original.endStates);
	return *this;
}

void Automaton::addStartState(Node* node) {
	startStates.push_back(node);
}

void Automaton::addEndState(Node* node) {
	endStates.push_back(node);
}

bool Automaton::feed(const std::string& value) {
	if(failed || value.empty())
		return false;
	std::vector<Node*> newStates;
	for (Node* node : curStates) {
		std::vector<Node*> nextStates = getNext(value, node);
		for (Node* newNode : nextStates) {
			bool nodeExists = false;
			for (Node* existingNode : newStates) {
				if (newNode == existingNode) {
					nodeExists = true;
					break;
				}
			}
			if (!nodeExists)
				newStates.push_back(newNode);
		}
	}
	if (newStates.empty()) {
		failed = true;
		return false;
	}
	curStates = newStates;
	doEpsilonMoves();
	
	return true;
}

void Automaton::doEpsilonMoves() {
	unsigned long long int curSize = curStates.size();
	
	for (Node* node : curStates) {
		std::vector<Node*> nextStates = getNext(EPSILON, node);
		for (Node* newNode : nextStates) {
			bool nodeExists = false;
			for (Node* existingNode : curStates) {
				if (newNode == existingNode) {
					nodeExists = true;
					break;
				}
			}
			if (!nodeExists)
				curStates.push_back(newNode);
		}
	}
	if (curStates.size() != curSize) {
		doEpsilonMoves();
	}
}

std::vector<Node*> Automaton::getNext(const std::string& condition, Node* current) {
	std::vector<Node*> nextNodes;
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

Node* Automaton::createNode(const std::string& id) {
	auto node = new Node(id);
	nodes.push_back(node);
	return node;
}

void Automaton::link(const std::string& condition, Node* from, Node* to) {
	auto it = links.find(from);
	if (it != links.end()) {
		it->second.emplace_back(Link(condition, to));
	} else {
		links.insert(std::pair<Node*, std::vector<Link>>(from, std::vector<Link>()));
		link(condition, from, to);
	}
}

void Automaton::link(const std::string& condition, Automaton* from, Automaton* to) {
	while (!from->nodes.empty()) {
		nodes.push_back(from->nodes[from->nodes.size() - 1]);
		from->nodes.pop_back();
	}
	while (!to->nodes.empty()) {
		nodes.push_back(to->nodes[to->nodes.size() - 1]);
		to->nodes.pop_back();
	}
	for (auto link : from->links) {
		links.insert(link);
	}
	for (auto link : to->links) {
		links.insert(link);
	}
	for (Node* end : from->endStates) {
		for (Node* start : to->startStates) {
			link(condition, end, start);
		}
	}
}

void Automaton::link(const std::string& condition, Automaton* from, Node* to) {
	while (!from->nodes.empty()) {
		nodes.push_back(from->nodes[from->nodes.size() - 1]);
		from->nodes.pop_back();
	}
	for (auto link : from->links) {
		links.insert(link);
	}
	for (Node* end : from->endStates) {
		link(condition, end, to);
	}
}

void Automaton::link(const std::string& condition, Node* from, Automaton* to) {
	while (!to->nodes.empty()) {
		nodes.push_back(to->nodes[to->nodes.size() - 1]);
		to->nodes.pop_back();
	}
	for (auto link : to->links) {
		links.insert(link);
	}
	for (Node* start : to->startStates) {
		link(condition, from, start);
	}
}

void Automaton::link(const std::string& inCondition, const std::string& outCondition, Node* entrance, Node* exit, Automaton* automata) {
	for (auto link : automata->links) {
		links.insert(link);
	}
	for (Node* start : automata->startStates) {
		link(inCondition, entrance, start);
	}
	for (Node* end : automata->endStates) {
		link(outCondition, end, exit);
	}
}

void Automaton::reset() {
	failed = false;
	curStates = std::vector<Node*>();
	curStates.reserve(startStates.size());
	for (Node* n : startStates)
		curStates.push_back(n);
	
	doEpsilonMoves();
}

bool Automaton::inEndState() {
	for (Node* node : curStates)
		for (Node* endState : endStates)
			if (node == endState)
				return true;
	
	return false;
}

bool Automaton::hasFailed() {
	return failed;
}

void Automaton::print() {
	for (Node* node : curStates)
		std::cout << "In node: \"" << node->id << "\"" << std::endl;
	
	if (inEndState()) {
		std::cout << "Valid end state!" << std::endl;
	} else {
		std::cout << "Invalid end state!" << std::endl;
	}
}

void Automaton::printLinks() {
	for (auto link : links) {
		for (Link target : link.second)
			std::cout << link.first->id << " -> (" << target.condition << ") -> " << target.next->id << std::endl;
	}
}
