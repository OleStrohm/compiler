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
	const std::string condition;
	Node* next;

	Link(const std::string& condition, Node* next);
	~Link();
};
