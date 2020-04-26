//
// Created by Ole on 05.12.2017.
//

#include "contextFreeGrammar.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include "../util/util.h"

std::string concatExpansion(const Expansion& expansion) {
	std::stringstream ss;
	for(auto& s : expansion.expansion) {
		ss << s << "|";
	}
	std::string res = ss.str();
	return std::move(res.substr(0, res.length()-1));
}

Expansion::Expansion(const std::string &expansionString) {
	std::stringstream ss;
	for (char c : expansionString) {
		if (util::isWhitespace(c)) {
			if (!util::isStringStreamEmpty(ss)) {
				expansion.push_back(std::move(ss.str())); // TODO: ??
				ss.str("");
			}
			continue;
		}
		if (util::isLetter(c)) {
			ss << c;
			continue;
		}
		if (!util::isStringStreamEmpty(ss)) {
			expansion.push_back(std::move(ss.str())); // TODO: ??
			ss.str("");
		}
		ss << c;
		expansion.push_back(std::move(ss.str())); // TODO: ??
		ss.str("");
	}
	if (!util::isStringStreamEmpty(ss)) {
		expansion.push_back(std::move(ss.str())); // TODO: ??
		ss.flush();
	}
}

CFG::CFG(const std::string &filepath) {
	std::ifstream file(filepath);
	std::string line;
	std::string current;
	while (std::getline(file, line)) {
		line = util::trim(line);
		if (line.empty() || line[0] == '#')
			continue;
		if (start.empty()) { // First non-empty non-comment line should contain the starting symbol
			start = std::move(line);
			continue;
		}
		if (line[line.length() - 1] == ':') { // Symbol definitions should have their own line with a colon at the end
			current = line.substr(0, line.length() - 1);
			if(std::find(symbols.begin(), symbols.end(), current) == symbols.end())
				symbols.push_back(current);
		} else if (line[0] == '|') { // After a symbol definition many lines of expansions can appear with empty/comment lines between
			if (current.empty()) {
				std::cerr << "Can't create expansion for non-existing symbol" << std::endl;
				return;
			}
			const std::string expansion = util::trim(line.substr(1));
//			printf("%s => %s\n", current.c_str(), expansion.c_str()); // debug
			addExpansion(current, expansion);
		}
	}

	std::cout << "Expansions:" << std::endl;
	for(auto& symbol : symbols) {
		auto& expansionList = this->expansions[symbol];
		for(auto& expansion : expansionList) {
			printf("%s => %s\n", symbol.c_str(), concatExpansion(expansion).c_str());
		}
	}

	std::cout << "\nStarting with: " << start << std::endl;
}

bool CFG::derive(const std::string& in) {

}

bool deriveStep(std::vector<std::string> stack, std::vector<std::string> derivation) {
	
}

void CFG::addExpansion(const std::string& parent, const std::string& expansion) {
	expansions[parent].emplace_back(expansion);
}