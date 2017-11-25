//
// Created by Ole on 16.11.2017.
//

#include <iostream>
#include "node.h"

Node::Node(const std::string& id)
		: id(id) {}

Link::Link(const char& condition, Node* next)
		: condition(condition), next(next) {}
