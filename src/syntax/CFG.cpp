//
// Created by Ole on 05.12.2017.
//

#include "CFG.h"

#include <fstream>
#include <iostream>

void parse(std::vector<std::string> lines);
std::string trim(const std::string& s);

CFG::CFG(std::string filepath) {
	std::ifstream file(filepath);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		line = trim(line);
		if (line.empty() || line[0] == '#')
			continue;
		if(lines.empty() || line[line.length() - 1] == ':') {
			lines.push_back(trim(line));
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
				lines.push_back(trim(line.substr(1)));
			} else {
				lastLine += " " + trim(line.substr(1));
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

std::string trim(const std::string& s) {
	if (s.empty())
		return "";
	unsigned long long int startPos = 0;
	unsigned long long int endPos = s.length() - 1;

	for (; startPos < s.length(); startPos++) {
		if (s.at(startPos) != '\t' && s.at(startPos) != ' ')
			break;
	}
	for (; endPos >= 0; endPos--) {
		if (s.at(endPos) != '\t' && s.at(endPos) != ' ')
			break;
	}
	return s.substr(startPos, endPos + 1 - startPos);
}
