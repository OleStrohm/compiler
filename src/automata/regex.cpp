//
// Created by Ole on 25.11.2017.
//

#include <fstream>
#include <vector>
#include <iostream>
#include "regex.h"

Regex::Regex(const std::string& path) {
	std::ifstream file(path);
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(file, line)) {
		lines.emplace_back(line);
	}
	
	createAutomaton(lines, "yeh");
}

void createAutomaton(std::vector<std::string> lines, const std::string& name) {

}
