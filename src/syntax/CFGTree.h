//
// Created by ole on 26/04/2020.
//

#pragma once

#include <vector>
#include <string>
#include "expansion.h"

enum class Color {
	black, red, green, yellow, blue, magenta, cyan, white
};

class TreeDisplayer {
	std::vector<std::vector<std::string>> screen;
	std::vector<std::string> paddings;

public:

	void printString(int x, int y, const std::string &s);

	void show();
};

class CFGTree {
private:
	std::vector<CFGTree *> children;
	std::string symbol;
	std::vector<std::string> expansion;

	void printTree(TreeDisplayer &display, int &x, int &y);

	void printNode(TreeDisplayer &display, const int &x, const int &y);

	static CFGTree *generateTree(const std::vector<Expansion> &expansions, int &index);

public:
	explicit CFGTree(Expansion expansion)
			: symbol(std::move(expansion.symbol)), expansion(std::move(expansion.symbols)) {}

	explicit CFGTree(std::string symbol)
			: symbol(std::move(symbol)) {}

	~CFGTree();

	void attachChild(CFGTree *child);

	TreeDisplayer printTree();

	static CFGTree *generateTree(const std::vector<Expansion> &expansions);
};
