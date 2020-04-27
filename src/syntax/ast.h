//
// Created by ole on 26/04/2020.
//

#pragma once

#include <vector>

class AST {
private:
	std::vector<AST*> children;

protected:
	virtual void printNode(int offset) = 0;
public:
	void attachChild(AST* child);

	void printTree(int offset = 0);
};
