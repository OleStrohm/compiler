//
// Created by ole on 26/04/2020.
//

#pragma once

#include <vector>
#include <string>
#include "expansion.h"
#include "treedisplay.h"

class CFGTree {
private:
	std::vector<CFGTree *> children;
	std::string symbol;
	std::vector<std::string> expansion;

	void printTree(TreeDisplay &display, int &x, int &y);

	void printNode(TreeDisplay &display, const int &x, const int &y);

	static CFGTree *generateTree(const std::vector<Expansion> &expansions, int &index);

public:
	explicit CFGTree(Expansion expansion)
			: symbol(std::move(expansion.symbol)), expansion(std::move(expansion.symbols)) {}

	explicit CFGTree(std::string symbol)
			: symbol(std::move(symbol)) {}

	~CFGTree();

	void attachChild(CFGTree *child);

	std::vector<CFGTree*>& getChildren() {
		return children;
	}

	std::string& getSymbol() {
		return symbol;
	}

	TreeDisplay printTree();

	static CFGTree *generateTree(const std::vector<Expansion> &expansions);
};
