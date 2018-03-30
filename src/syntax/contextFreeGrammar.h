//
// Created by Ole on 05.12.2017.
//

#pragma once

#include <vector>
#include <string>

struct Rule {
	std::string parent, child;
	
	Rule(std::string parent, std::string child)
			: parent(std::move(parent)), child(std::move(child)) {}
};

struct DerivationNode {
	std::string value;
	bool isTerminating;
	
	DerivationNode(std::string value, bool isTerminating)
			: value(std::move(value)), isTerminating(isTerminating) {}
};

class CFG {
private:
	std::string start;
	std::vector<Rule> rules;
	std::vector<std::string> nonTerminatingSymbols;
	void createRules(std::vector<std::string> lines);
	
	inline bool isTerminatingSymbol(std::string s) {
		for(int i = 0; i < nonTerminatingSymbols.size(); i++) {
			if(nonTerminatingSymbols[i] == s)
				return false;
		}
		return true;
	}
	
	bool deriveStep(std::string text, std::vector<DerivationNode> nodes);
public:
	CFG(std::string filepath);
	
	bool derive(std::string text);
};


