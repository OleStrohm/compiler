//
// Created by Ole on 16.11.2017.
//

#pragma once

#include <vector>

struct Link;

class Node {
public:
	const std::string id;

	explicit Node(const std::string& id);
};

struct Link {
	const char condition;
	Node* next;

	Link(const char& condition, Node* next);
};
