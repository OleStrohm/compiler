//
// Created by ole on 26/04/2020.
//

#include <iostream>
#include "CFGTree.h"

void TreeDisplayer::printString(int x, int y, const std::string& s) {
	while(screen.size() < y + 1) {
		if(screen.empty())
			screen.emplace_back();
		else
			screen.push_back(screen[screen.size()-1]);
	}
	std::vector<std::string>& row = screen[y];
	if(row.size() < x + 1) {
		for(auto& row2 : screen)
			while (row2.size() < x + 1) {
				row2.emplace_back();
				paddings.emplace_back();
			}
	}
	std::string& padding = paddings[x];
	while(padding.length() < s.length())
		padding += ' ';
	while(y < screen.size())
		screen[y++][x] = s;
}

void TreeDisplayer::show() {
	for(auto& row : screen) {
		std::cout << "| ";
		for(int x = 0; x < row.size(); x++) {
			const std::string& s = row[x];
			int padding = paddings[x].length() - s.length();
			x++;
			while(x < row.size() && row[x].empty()) {
				padding += paddings[x].length() + 3;
				x++;
			}
			x--;
			int halfPadding = padding/2;
			std::string newPadding;
			while(newPadding.length() < halfPadding)
				newPadding += ' ';
			std::cout << newPadding << s << newPadding << (halfPadding * 2 < padding ? " " : "") << " | ";
		}
		std::cout << std::endl;
	}
}

CFGTree::~CFGTree() {
	for (auto child : children)
		delete child;
}

void CFGTree::attachChild(CFGTree *child) {
	children.push_back(child);
}

void CFGTree::printNode(TreeDisplayer& display, const int& x, const int& y) {
	display.printString(x, y, symbol);
//	std::cout << "Print '" << symbol << "' at (" << x << ", " << y << ")" << std::endl;
}

void CFGTree::printTree(TreeDisplayer& display, int& x, int& y) {
	printNode(display, x, y);
	y++;
	for (auto child : children) {
		child->printTree(display, x, y);
		if(child->children.empty())
			x++;
	}
	y--;
}

TreeDisplayer CFGTree::printTree() {
	TreeDisplayer display;
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