//
// Created by ole on 26/04/2020.
//

#include <iostream>
#include "CFGTree.h"

CFGTree::~CFGTree() {
	for (auto child : children)
		delete child;
}

void CFGTree::attachChild(CFGTree *child) {
	children.push_back(child);
}

void CFGTree::printNode(TreeDisplay &display, const int &x, const int &y) {
	display.printString(x, y, symbol);
//	std::cout << "Print '" << symbol << "' at (" << x << ", " << y << ")" << std::endl;
}

void CFGTree::printTree(TreeDisplay &display, int &x, int &y) {
	printNode(display, x, y);
	y++;
	for (auto child : children) {
		child->printTree(display, x, y);
		if (child->children.empty())
			x++;
	}
	y--;
}

TreeDisplay CFGTree::printTree() {
	TreeDisplay display;
	int x = 0, y = 0;
	printTree(display, x, y);
	return display;
}

CFGTree *CFGTree::generateTree(const std::vector<Expansion> &expansions, int &index) {
	auto node = new CFGTree(expansions[index++]);

	for (auto &s : node->expansion) {
		if (s == expansions[index].symbol) {
			node->attachChild(generateTree(expansions, index));
		} else {
			node->attachChild(new CFGTree(s));
		}
	}

	return node;
}

CFGTree *CFGTree::generateTree(const std::vector<Expansion> &expansions) {
	int index = 0;
	return generateTree(expansions, index);
}