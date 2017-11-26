//
// Created by Ole on 25.11.2017.
//

#include <fstream>
#include <vector>
#include <iostream>
#include "regex.h"
#include "automata.h"

#define LOG(x) std::cout << x << std::endl;

Automata createAutomaton(const std::vector<std::string>& lines, const std::string& name);
std::string trim(const std::string& s);
std::string shortenWhitespace(const std::string& s);
std::string reformatText(const std::string& s);
bool startsWith(const std::string& s, const std::string& test);

Regex::Regex(const std::string& path) {
	std::ifstream file(path);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		} else if (!lines.empty() && line.at(0) == '\t' && lines[lines.size() - 1].at(0) == '\t') {
			lines[lines.size() - 1] = lines[lines.size() - 1] + " | (" + line.substr(1) + ")";
		} else {
			lines.emplace_back(line);
		}
	}
	
	createAutomaton(lines, shortenWhitespace(trim(lines[1])));
}

Automata createAutomaton(const std::vector<std::string>& lines, const std::string& name) {
	LOG("Parsing: \"" << name << "\"");
	int openParentheses = 0;
	bool inText = false;
	
	auto orPos = std::string::npos;
	for (unsigned long long int p = 0; p < name.length() - 1; p++) {
		if (name.at(p) == '(')
			openParentheses++;
		if (name.at(p) == ')')
			openParentheses--;
		if(name.at(p) == '.')
			inText = true;
		if(name.at(p) == ' ')
			inText = false;
		
		if (!inText && openParentheses == 0 && name.at(p) == '|') {
			orPos = p;
			break;
		}
	}
	if(orPos != std::string::npos) { // Todo: Do you want this or that?
		Automata left = createAutomaton(lines, trim(name.substr(0, orPos)));
		Automata right = createAutomaton(lines, trim(name.substr(orPos + 1)));
		Automata concatenatedAutomaton;
		concatenatedAutomaton.link(EPSILON, &left, &right);
		return concatenatedAutomaton;
	}
	
	auto concatenatePos = std::string::npos;
	openParentheses = 0;
	for (unsigned long long int p = 0; p < name.length() - 1; p++) {
		if (name.at(p) == '(')
			openParentheses++;
		if (name.at(p) == ')')
			openParentheses--;
		
		if (openParentheses == 0 && name.at(p) == ' ' && name.at(p + 1) != '|' && name.at(p + 1) != '&' && name.at(p + 1) != '*' && name.at(p + 1) != '+' && name.at(p + 1) != '?'
											 && p > 0 && name.at(p - 1) != '|' && name.at(p - 1) != '&' && name.at(p + 1) != '*' && name.at(p + 1) != '+' && name.at(p + 1) != '?') {
			concatenatePos = p;
			break;
		}
	}
	
	if (concatenatePos != std::string::npos) { // Todo: concatenate everything
		createAutomaton(lines, trim(name.substr(0, concatenatePos)));
		createAutomaton(lines, trim(name.substr(concatenatePos + 1)));
		concatenatedAutomata;
		return;
	}
	
	auto unionPos = std::string::npos;
	openParentheses = 0;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '(')
			openParentheses++;
		if (name.at(p) == ')')
			openParentheses--;
		if(name.at(p) == '.')
			inText = true;
		if(name.at(p) == ' ')
			inText = false;
		
		if (!inText && openParentheses == 0 && name.at(p) == '&') {
			unionPos = p;
			break;
		}
	}
	if (unionPos != std::string::npos) { // Todo: union these sons of bitches
		createAutomaton(lines, trim(name.substr(0, unionPos)));
		createAutomaton(lines, trim(name.substr(unionPos + 1)));
		return;
	}
	
	auto kleenePos = std::string::npos;
	openParentheses = 0;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '(')
			openParentheses++;
		if (name.at(p) == ')')
			openParentheses--;
		if(name.at(p) == '.')
			inText = true;
		if(name.at(p) == ' ')
			inText = false;
		
		if (!inText && openParentheses == 0 && name.at(p) == '*') {
			kleenePos = p;
			break;
		}
	}
	if (kleenePos != std::string::npos) { // Todo: LOOP THE FUCKING LOOP
		createAutomaton(lines, trim(name.substr(0, kleenePos)));
		return;
	}
	
	auto plusPos = std::string::npos;
	openParentheses = 0;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '(')
			openParentheses++;
		if (name.at(p) == ')')
			openParentheses--;
		if(name.at(p) == '.')
			inText = true;
		if(name.at(p) == ' ')
			inText = false;
		
		if (!inText && openParentheses == 0 && name.at(p) == '+') {
			plusPos = p;
			break;
		}
	}
	if (plusPos != std::string::npos) { // Todo: ONCE + LOOP THE FUCKING LOOP
		createAutomaton(lines, trim(name.substr(0, plusPos)));
		return;
	}
	
	auto questionMarkPos = std::string::npos;
	openParentheses = 0;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '(')
			openParentheses++;
		if (name.at(p) == ')')
			openParentheses--;
		if(name.at(p) == '.')
			inText = true;
		if(name.at(p) == ' ')
			inText = false;
		
		if (!inText && openParentheses == 0 && name.at(p) == '?') {
			questionMarkPos = p;
			break;
		}
	}
	if (questionMarkPos != std::string::npos) { // Todo: ONCE OR NAH
		createAutomaton(lines, trim(name.substr(0, questionMarkPos)));
		return;
	}
	
	if (name.at(0) == '(' && name.at(name.length() - 1) == ')') {
		createAutomaton(lines, trim(name.substr(1, name.length() - 2)));
		return;
	}
	
	if (name.at(0) == '.') { // todo: create text Automaton.
		LOG("\tI want to find the text: \"" << reformatText(name.substr(1)) << "\"");
		return;
	} else { // todo: parse new regex
		LOG("\tI'm gonna be replaced by the Automata called: \"" << name << "\"");
		int index = 1;
		for(const std::string& line : lines) {
			if(trim(line)[line.length()-1] == ':') {
				if(startsWith(line, name))
					break;
			}
			index++;
		}
		if(index >= lines.size()) {
			std::cerr << "Couldn't find regex: \"" << name << "\"" << std::endl;
			return;
		}
		createAutomaton(lines, shortenWhitespace(trim(lines[index])));
		return;
	}
}

std::string trim(const std::string& s) {
	unsigned long long int startPos = 0;
	unsigned long long int endPos = s.length() - 1;
	
	for(unsigned long long int sp = 0; sp < s.length(); sp++) {
		if(s.at(sp) != '\t' && s.at(sp) != ' ') {
			startPos = sp;
			break;
		}
	}
	for(unsigned long long int ep = s.length()-1; ep >= 0; ep--) {
		if(s.at(ep) != '\t' && s.at(ep) != ' ') {
			endPos = ep;
			break;
		}
	}
	return s.substr(startPos, endPos + 1);
}

std::string shortenWhitespace(const std::string& s) {
	std::string res;
	bool inWhitespace = false;
	
	for (char c : s) {
		if(c == '\t' || c == ' ')
			inWhitespace = true;
		else if(inWhitespace) {
			inWhitespace = false;
			res += " ";
			res += c;
		} else {
			res += c;
		}
	}
	
	return res;
}

std::string reformatText(const std::string& s) {
	std::string result = s;
	unsigned long long int replacePos = result.find("\\s");
	while(replacePos != std::string::npos) {
		result = result.replace(replacePos, 2, " ");
		replacePos = result.find("\\s", replacePos);
	}
	
	replacePos = result.find("\\n");
	while(replacePos != std::string::npos) {
		result = result.replace(replacePos, 2, "\n");
		replacePos = result.find("\\n", replacePos);
	}
	
	replacePos = result.find("\\t");
	while(replacePos != std::string::npos) {
		result = result.replace(replacePos, 2, "\t");
		replacePos = result.find("\\t", replacePos);
	}
	
	replacePos = result.find("\\\\");
	while(replacePos != std::string::npos) {
		result = result.replace(replacePos, 2, "\\");
		replacePos = result.find("\\\\", replacePos);
	}
	return result;
}

bool startsWith(const std::string& s, const std::string& test) {
	if(test.length() > s.length())
		return false;
	for(int i = 0; i < test.length(); i++) {
		if(s[i] != test[i])
			return false;
	}
	return true;
}

















































