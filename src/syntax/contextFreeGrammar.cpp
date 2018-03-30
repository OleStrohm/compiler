//
// Created by Ole on 05.12.2017.
//

#include "contextFreeGrammar.h"

#include <fstream>
#include <iostream>
#include "../util/util.h"

CFG::CFG(std::string filepath) {
	std::ifstream file(filepath);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		line = util::trim(line);
		if (line.empty() || line[0] == '#')  // useless lines / comments
			continue;
		if (line[line.length() - 1] == ':') { // New symbol (well it doesn't technically have to be new but whatever
			lines.push_back(line);
		} else if (line[0] == '|') {             // A NEW RULE
			std::string& lastLine = lines.back();
			if (lastLine[lastLine.size() - 1] == ':') { // Add it on a new line
				lines.push_back(util::trim(line.substr(1)));
			} else {
				lastLine += " " + line;
			}
		} else {
			start = util::trim(line);
		}
	}

//	for(auto& line : lines)
//		std::cout << line << std::endl;
//	std::cout << "<EOF>" << std::endl << std::endl;
	
	createRules(lines);
	
	std::cout << "Rules: (Starts at \'" << start << "\')" << std::endl;
	for (int i = 0; i < rules.size(); i++) {
		std::cout << "  " << rules[i].parent << " -> " << rules[i].child << std::endl;
	}
	std::cout << "Rules end" << std::endl << std::endl;
}

bool CFG::deriveStep(std::string text, std::vector<DerivationNode> nodes) {
	if(nodes.size() > 3) // TODO: Guess once.
		return false;
	std::cout << "Current nodes:" << std::endl;
	for (auto& node : nodes) {
		std::cout << node.value << " (" << (node.isTerminating ? "Terminating)" : "Not terminating)") << std::endl;
	}
	std::cout << "End of nodes" << std::endl;
	
	for(int i = 0; i < nodes.size(); i++) {
		if(!nodes[i].isTerminating) {
			for(int j = 0; j < rules.size(); j++) {
				std::vector<DerivationNode> newNodes = nodes;
				bool inText = false;
				size_t lastStop = 0;
				for (size_t i = 0; i < text.size(); i++) {
					if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122)) {
						if (!inText) {
							nodes.emplace_back(text.substr(lastStop, i - lastStop), true);
							lastStop = i;
						}
						inText = true;
					} else if (inText) {
						inText = false;
						
						nodes.emplace_back(text.substr(lastStop, i - lastStop), isTerminatingSymbol(text.substr(lastStop, i - lastStop)));
						lastStop = i;
					}
				}
				if (lastStop < text.size() - 1) {
					nodes.emplace_back(text.substr(lastStop), isTerminatingSymbol(text.substr(lastStop)));
				}
				newNodes.erase(newNodes.begin() + i);
				if(deriveStep(text, newNodes))
					return true;
			}
		}
	}
	
	// TODO: check if shit actually satisfies the shit or whatevz
	
	return false;
}

bool CFG::derive(std::string text) {
	std::vector<DerivationNode> nodes;
	
	nodes.emplace_back(start, isTerminatingSymbol(start));
	
//	bool inText = false; // TODO: I need this
//	size_t lastStop = 0;
//	for (size_t i = 0; i < text.size(); i++) {
//		if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122)) {
//			if (!inText) {
//				nodes.emplace_back(text.substr(lastStop, i - lastStop), true);
//				lastStop = i;
//			}
//			inText = true;
//		} else if (inText) {
//			inText = false;
//
//			nodes.emplace_back(text.substr(lastStop, i - lastStop), isTerminatingSymbol(text.substr(lastStop, i - lastStop)));
//			lastStop = i;
//		}
//	}
//	if(lastStop < text.size() - 1) {
//		nodes.emplace_back(text.substr(lastStop), isTerminatingSymbol(text.substr(lastStop)));
//	}
	
	std::cout << "Deriving the tree for: \'" << text << "\'" << std::endl;
	return deriveStep(text, nodes);
}

void CFG::createRules(std::vector<std::string> lines) {
	std::string parent;
	for (auto& line : lines) {
		if (line[line.length() - 1] == ':') {
			parent = line.substr(0, line.length() - 1);
			bool containsParent = false;
			for (int i = 0; i < nonTerminatingSymbols.size(); i++) {
				if (nonTerminatingSymbols[i] == parent) {
					containsParent = true;
					break;
				}
			}
			if (!containsParent)
				nonTerminatingSymbols.push_back(parent);
		} else {
			size_t lastOrPos = 0;
			size_t orPos = 0;
			while ((orPos = line.find('|', lastOrPos + 1)) != std::string::npos) {
				rules.emplace_back(parent, util::trim(line.substr(lastOrPos, orPos - lastOrPos)));
				lastOrPos = orPos + 1;
			}
			rules.emplace_back(parent, util::trim(line.substr(lastOrPos)));
		}
	}
}
