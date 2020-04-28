//
// Created by ole on 26/04/2020.
//

#pragma once

#include <vector>
#include "CFGTree.h"
#include "treedisplay.h"

class AST {
protected:
	virtual void printNode(TreeDisplay& display, int x, int y) = 0;
public:
	virtual std::string code() = 0;
	virtual void printTree(TreeDisplay &display, int &x, int &y) = 0;
	TreeDisplay printTree();

	static AST* generateAST(CFGTree* parseTree);
};