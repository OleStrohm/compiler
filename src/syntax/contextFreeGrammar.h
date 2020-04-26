//
// Created by Ole on 05.12.2017.
//

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <map>

struct Expansion {
	std::vector<std::string> expansion;

	explicit Expansion(const std::string&);
};

class CFG {
private:
	std::string start;
	std::map<std::string, std::vector<Expansion>> expansions;
	std::vector<std::string> symbols;

	inline bool isTerminatingSymbol(const std::string& s) {
		for(int i = 0; i < symbols.size(); i++) { // TODO: Binary search on a ordered list
			if(symbols[i] == s)
				return false;
		}
		return true;
	}

	void addExpansion(const std::string& parent, const std::string& expansion);

	bool deriveStep(std::vector<std::string> stack, std::vector<std::string> derivation);
public:
	explicit CFG(const std::string& filepath);

	bool derive(const std::string& in);
};


