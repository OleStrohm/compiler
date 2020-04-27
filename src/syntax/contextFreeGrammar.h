//
// Created by Ole on 05.12.2017.
//

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "expansion.h"

class CFG {
private:
	std::string start;
	std::map<std::string, std::vector<Expansion>> expansions;
	std::vector<std::string> nonTerminatingSymbols;

	inline bool isTerminatingSymbol(const std::string& s) {
		for(int i = 0; i < nonTerminatingSymbols.size(); i++) { // TODO: Binary search on a ordered list
			if(nonTerminatingSymbols[i] == s)
				return false;
		}
		return true;
	}

	void addExpansion(const std::string& parent, const std::string& expansion);

	bool validateStep(std::vector<std::string> stack, std::vector<std::string> derivation,
					  std::vector<Expansion>& stacktrace);
public:
	explicit CFG(const std::string& filepath);

	bool validate(const std::string& in, std::vector<Expansion>& stacktrace);
};


