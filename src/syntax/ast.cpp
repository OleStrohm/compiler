//
// Created by ole on 26/04/2020.
//

#include "ast.h"
#include <iostream>

void AST::attachChild(AST* child) {
	children.push_back(child);
}

void AST::printTree(int offset) {
	printNode(offset);
	std::cout << "(" << std::endl;
	for(auto child : children)
		child->printTree(offset + 1);
	std::cout << ")" << std::endl;
}