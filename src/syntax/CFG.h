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

class CFG {
private:
	std::vector<Rule> rules;
public:
	CFG(std::string filepath);
};


