//
// Created by Ole on 05.12.2017.
//

#include "contextFreeGrammar.h"

#include <fstream>
#include <iostream>
#include "../util/util.h"

void parse(std::vector<std::string> lines);

CFG::CFG(std::string filepath) {
	std::ifstream file(filepath);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		line = util::trim(line);
		if (line.empty() || line[0] == '#')
			continue;
		if(lines.empty() || line[line.length() - 1] == ':') {
			lines.push_back(util::trim(line));
		} else if(line[0] == '|') {
			std::string& lastLine = lines.back();
			if(lastLine[lastLine.size() - 1] == ':') {
				lines.push_back(line);
			} else {
				lastLine += " " + line;
			}
		} else if(line[0] == ':') {
			std::string& lastLine = lines.back();
			if(lastLine[lastLine.size() - 1] == ':') {
				lines.push_back(util::trim(line.substr(1)));
			} else {
				lastLine += " " + util::trim(line.substr(1));
			}
		}
	}

	for(auto& line : lines)
		std::cout << line << std::endl;
	std::cout << "<EOF>" << std::endl << std::endl;

	parse(lines);
}

void parse(std::vector<std::string> lines) {
	std::string parent;
	for(auto& line : lines) {
		if(line[line.length()-1] == ':') {
			parent = line.substr(0, line.length()-1);
		} else {

		}
	}
}
