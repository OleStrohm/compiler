//
// Created by ole on 26/04/2020.
//

#include "ast.h"
#include <iostream>
#include <sstream>
#include <utility>

TreeDisplay AST::printTree() {
	TreeDisplay display;
	int x = 0, y = 0;
	printTree(display, x, y);
	return std::move(display);
}

class Unknown : public AST {
	std::string expansionSymbol;

public:
	explicit Unknown(std::string expansionSymbol)
			: expansionSymbol(std::move(expansionSymbol)) {}

	void printTree(TreeDisplay &display, int &x, int &y) override {
		printNode(display, x, y);
		x++;
	}

	std::string code() override {
		return "/*Unknown Node*/";
	}

protected:
	void printNode(TreeDisplay &display, int x, int y) override {
		std::stringstream ss;
		ss << "Unknown(" << expansionSymbol << ")";
		display.printString(x, y, ss.str());
	}
};

class BinOp : public AST {
	AST *left;
	AST *right;
	std::string op;

public:
	void printTree(TreeDisplay &display, int &x, int &y) override {
		printNode(display, x, y);
		y++;
		left->printTree(display, x, y);
		right->printTree(display, x, y);
		y--;
	}

	BinOp(AST *left, std::string op, AST *right)
			: left(left), op(std::move(op)), right(right) {}

protected:
	void printNode(TreeDisplay &display, int x, int y) override {
		display.printString(x, y, op);
	}

public:
	std::string code() override {
		std::stringstream ss;
		ss << "(" << left->code() << " " << op << " " << right->code() << ")";
		return ss.str();
	}
};

class Atom : public AST {
	std::string value;
	bool literal;

	void printTree(TreeDisplay &display, int &x, int &y) override {
		printNode(display, x, y);
		x++;
	}

protected:
	void printNode(TreeDisplay &display, int x, int y) override {
		display.printString(x, y, value);
	}

public:
	Atom(std::string value, bool literal)
			: value(std::move(value)), literal(literal) {}

	std::string code() override {
		return value;
	}
};

class IfStatement : public AST {
	AST *condition;
	AST *then;

public:
	IfStatement(AST *condition, AST *then)
			: condition(condition), then(then) {}

	void printTree(TreeDisplay &display, int &x, int &y) override {
		printNode(display, x, y);
		y++;
		condition->printTree(display, x, y);
		then->printTree(display, x, y);
		y--;
	}

public:
	std::string code() override {
		std::stringstream ss;
		ss << "if (" << condition->code() << ") " << then->code();
		return ss.str();
	}

protected:
	void printNode(TreeDisplay &display, int x, int y) override {
		display.printString(x, y, "if");
	}
};

class StatementList : public AST {
	std::vector<AST *> statements;

public:
	void printTree(TreeDisplay &display, int &x, int &y) override {
		printNode(display, x, y);

		y++;
		for (auto &s : statements) {
			s->printTree(display, x, y);
		}
		y--;
	}

	std::string code() override {
		std::stringstream ss;
		ss << "{\n";
		for(auto& s : statements)
			ss << s->code() << "\n";
		ss << "}";
		return ss.str();
	}

	void appendStatement(AST *statement) {
		statements.push_back(statement);
	}

protected:
	void printNode(TreeDisplay &display, int x, int y) override {
		display.printString(x, y, "Block");
	}
};

class Assignment : public AST {
	AST *variable;
	std::string op;
	AST *value;

public:
	std::string code() override {
		std::stringstream ss;
		ss << variable->code() << " " << op << " " << value->code() << ";";
		return ss.str();
	}

	Assignment(AST *variable, std::string op, AST *expression)
			: variable(variable), op(std::move(op)), value(expression) {
	}

	void printTree(TreeDisplay &display, int &x, int &y) override {
		printNode(display, x, y);
		y++;
		variable->printTree(display, x, y);
		value->printTree(display, x, y);
		y--;
	}

protected:
	void printNode(TreeDisplay &display, int x, int y) override {
		display.printString(x, y, op);
	}

};

AST *AST::generateAST(CFGTree *parseTree) {
	AST *node = nullptr;

	auto &children = parseTree->getChildren();
	unsigned int numChildren = children.size();
	auto& symbol = parseTree->getSymbol();

	if (numChildren == 0) {
		node = new Atom(symbol, symbol != "id");
	} else if (symbol == "IfStatement") {
		auto condition = children[2];
		auto then = children[5];
		node = new IfStatement(generateAST(condition),
							   generateAST(then));
	} else if (symbol == "StatementList") {
		auto lookahead = parseTree;
		node = new StatementList;
		while (lookahead->getChildren().size() > 1) {
			((StatementList *) node)->appendStatement(generateAST(lookahead->getChildren()[0]));
			lookahead = lookahead->getChildren()[1];
		}
		((StatementList *) node)->appendStatement(generateAST(lookahead->getChildren()[0]));
	} else if (symbol == "Assignment") {
		node = new Assignment(
				generateAST(children[0]),
				children[1]->getSymbol(),
				generateAST(children[2])
		);
	} else if ((symbol == "Expr" || symbol == "Factor") && numChildren == 3) {
		node = generateAST(children[1]);
	} else if (numChildren == 1) {
		node = generateAST(children[0]);
	} else if (numChildren == 3) {
		node = new BinOp(generateAST(children[0]),
						 children[1]->getSymbol(),
						 generateAST(children[2]));
	} else {
		node = new Unknown(parseTree->getSymbol());
	}

	return node;
}