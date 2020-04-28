//
// Created by Ole on 05.12.2017.
//

#include "contextFreeGrammar.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include "../util/util.h"

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
			if (std::find(nonTerminatingSymbols.begin(), nonTerminatingSymbols.end(), current) ==
				nonTerminatingSymbols.end())
				nonTerminatingSymbols.push_back(current);
		} else if (line[0] ==
				   '|') { // After a symbol definition many lines of expansions can appear with empty/comment lines between
			if (current.empty()) {
				std::cerr << "Can't create nonTerminatingSymbols for non-existing symbol" << std::endl;
				return;
			}
			const std::string expansion = util::trim(line.substr(1));
//			printf("%s => %s\n", current.c_str(), expansion.c_str()); // debug
			addExpansion(current, expansion);
		}
	}

	std::cout << "Starting with: " << start << std::endl;
	std::cout << "Expansions:" << std::endl;
	for (auto &symbol : nonTerminatingSymbols) {
		auto &expansionList = this->expansions[symbol];
		for (auto &expansion : expansionList) {
			std::cout << "  " << expansion << std::endl;
		}
	}

}

bool CFG::validate(const std::string &in, std::vector<Expansion> &stacktrace) {
	Expansion stack(start, in);
	std::vector<std::string> derivation;
	derivation.push_back(start);
	std::cout << std::endl << "Validating: '" << concatExpansion(stack.symbols) << "'" << std::endl;
	return validateStep(stack.symbols, derivation, stacktrace);
}

bool CFG::validateStep(std::vector<std::string> stack, std::vector<std::string> derivation,
					   std::vector<Expansion> &stacktrace) {
//	std::cout << "\tNext step" << std::endl;
//	std::cout << "Stack: " << concatExpansion(stack) << std::endl;
//	std::cout << "Derivation: " << concatExpansion(derivation) << std::endl;

	while (!derivation.empty() && isTerminatingSymbol(derivation[0])) {
//		printf("Comparing '%s' > '%s'\n", stack[0].c_str(), derivation[0].c_str());
		if (stack[0] != derivation[0]) {
//			std::cout << "Branch failed, backtracking" << std::endl;
			return false;
		}

		stack.erase(stack.begin());
		derivation.erase(derivation.begin());
	}
	if (derivation.empty()) {
		return stack.empty();
	}
//	std::cout << "Pruned Stack: " << concatExpansion(stack) << std::endl;
//	std::cout << "Pruned Derivation: " << concatExpansion(derivation) << std::endl;

//	std::cout << "\tChecking possibilites" << std::endl;
	std::string current = derivation[0];
	derivation.erase(derivation.begin());
	for (auto &expansion : expansions[current]) {
		if (derivation.size() + expansion.symbols.size() > stack.size()) {
//			std::cout << "\nSkipping possibility (Too long): " << concatExpansion(expansion) << std::endl;
			continue;
		}
//		std::cout << "\nTesting possibility: " << concatExpansion(expansion) << std::endl;
		for (int s = 0; s < expansion.symbols.size(); s++)
			derivation.insert(derivation.begin() + s, expansion.symbols[s]);
		bool res = validateStep(stack, derivation, stacktrace);
		if (res) {
			stacktrace.push_back(expansion);
			return true;
		}
		for (int s = 0; s < expansion.symbols.size(); s++)
			derivation.erase(derivation.begin());
	}

	return false;
}

void CFG::addExpansion(const std::string &parent, const std::string &expansion) {
	expansions[parent].emplace_back(parent, expansion);
}