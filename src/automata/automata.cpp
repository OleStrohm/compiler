//
// Created by Ole on 16.11.2017.
//

#include <iostream>
#include "automata.h"

Automata Automata::nullAutomata("null");

Automata::Automata(const std::string& id)
		: id(id) {}

Automata::Automata(const Automata& original)
		: id(original.id) {}

Automata::Automata(Automata&& original)
		: id(original.id) {
	std::swap(id, original.id);
	std::swap(nodes, original.nodes);
	std::swap(links, original.links);
	std::swap(startStates, original.startStates);
	std::swap(endStates, original.endStates);
}

Automata::~Automata() {
	//std::cout << "Deleted Automata: " << id << std::endl;
	//printLinks();
	std::cout << std::endl;
	
	for (Node* node : nodes) {
		delete node;
	}
}

Automata& Automata::operator=(const Automata& original) {
	this->id = original.id;
	return *this;
}

Automata& Automata::operator =(Automata&& original) {
	std::swap(id, original.id);
	std::swap(nodes, original.nodes);
	std::swap(links, original.links);
	std::swap(startStates, original.startStates);
	std::swap(endStates, original.endStates);
	return *this;
}

void Automata::addStartState(Node* node) {
	startStates.push_back(node);
}

void Automata::addEndState(Node* node) {
	endStates.push_back(node);
}

bool Automata::feed(const std::string& value) {
	if (value.length() <= 0)
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
	if (newStates.empty())
		return false;
	curStates = newStates;
	doEpsilonMoves();
	
	std::cout << "Going to:" << std::endl;
	for (Node* node : curStates)
		std::cout << "In node: \"" << node->id << "\"" << std::endl;
	std::cout << std::endl;
	
	return true;
}

void Automata::doEpsilonMoves() {
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

std::vector<Node*> Automata::getNext(const std::string& condition, Node* current) {
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

Node* Automata::createNode(const std::string& id) {
	auto node = new Node(id);
	nodes.push_back(node);
	return node;
}

void Automata::link(const std::string& condition, Node* from, Node* to) {
	auto it = links.find(from);
	if (it != links.end()) {
		it->second.emplace_back(Link(condition, to));
	} else {
		links.insert(std::pair<Node*, std::vector<Link>>(from, std::vector<Link>()));
		link(condition, from, to);
	}
}

void Automata::link(const std::string& condition, Automata* from, Automata* to) {
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

void Automata::link(const std::string& condition, Automata* from, Node* to) {
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

void Automata::link(const std::string& condition, Node* from, Automata* to) {
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

void Automata::link(const std::string& inCondition, const std::string& outCondition, Node* entrance, Node* exit, Automata* automata) {
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

void Automata::reset() {
	curStates = std::vector<Node*>();
	curStates.reserve(startStates.size());
	for (Node* n : startStates)
		curStates.push_back(n);
}

bool Automata::inEndState() {
	for (Node* node : curStates)
		for (Node* endState : endStates)
			if (node == endState)
				return true;
	
	return false;
}

void Automata::print() {
	for (Node* node : curStates)
		std::cout << "In node: \"" << node->id << "\"" << std::endl;
	
	if (inEndState()) {
		std::cout << "Valid end state!" << std::endl;
	} else {
		std::cout << "Invalid end state!" << std::endl;
	}
}

void Automata::printLinks() {
	for (auto link : links) {
		for (Link target : link.second)
			std::cout << link.first->id << " -> (" << target.condition << ") -> " << target.next->id << std::endl;
	}
}
