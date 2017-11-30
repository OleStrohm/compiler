//
// Created by Ole on 25.11.2017.
//

#include <fstream>
#include <vector>
#include <iostream>
#include "regex.h"
#include "automata.h"

Automaton createAutomaton(const std::vector<std::string>& lines, const std::string& name);
std::string trim(const std::string& s);
std::string shortenWhitespace(const std::string& s);
std::string reformatText(const std::string& s);
bool startsWith(const std::string& s, const std::string& test);

Regex::Regex(const std::string& path) {
	std::ifstream file(path);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		line = trim(line);
		const std::string& lastLine = lines[lines.size() - 1];
		if (line.empty() || line[0] == '#') {
			continue;
		} else if (!lines.empty() && !(line[line.length() - 1] == '>' || line[line.length() - 1] == ':') && !(lines[lines.size() - 1][lines[lines.size() - 1].length() - 1] == '>' || lines[lines.size() - 1][lines[lines.size() - 1].length() - 1] == ':')) {
			lines[lines.size() - 1] = lines[lines.size() - 1] + " | " + line;
		} else {
			lines.emplace_back(line);
		}
	}
	
//	for (auto& line : lines) {
//		std::cout << shortenWhitespace(line) << std::endl;
//	}
	std::vector<unsigned int> regexes;
	unsigned int index = 1;
	for (auto& line : lines) {
		if (line[line.length() - 1] == '>') {
			regexes.emplace_back(index);
			names.emplace_back(line.substr(0, line.length() - 1));
		}
		index++;
	}

	names.emplace_back("error");

	automata.reserve(regexes.size());

	for (unsigned int i : regexes)
		automata.push_back(createAutomaton(lines, shortenWhitespace(trim(lines[i]))));
}

Token Regex::extractToken(const std::string& text, const unsigned int start, unsigned int* end) {
	for (auto& a : automata) {
		a.reset();
	}
	
	std::vector<bool> validAutomatons(automata.size(), true);
	unsigned int validCount = (int) automata.size();
	unsigned int pos = start;
	
	while (validCount > 0 && pos < text.length()) {
		int index = 0;
		for (auto& a : automata) {
			a.feed(std::string(1, text[pos]));
			if (a.hasFailed() && validAutomatons[index]) {
				validCount--;
			}
			index++;
		}
		if (validCount == 0)
			break;
		index = 0;
		for (auto& a : automata) {
			if (a.hasFailed()) {
				validAutomatons[index] = false;
			}
			index++;
		}
		pos++;
	}
	*end = pos;
	
	if (pos != start)
		for (int i = 0; i < validAutomatons.size(); i++) {
			if (!validAutomatons[i] || !automata[i].inEndState())
				continue;
			
			return {names[i], text.substr(start, pos - start)};
		}
	(*end)++;
	return {names[names.size() - 1], text.substr(start, (*end) - start)};
}

Automaton createAutomaton(const std::vector<std::string>& lines, const std::string& name) {
	int openParentheses = 0;
	bool inText = false;
	
	auto orPos = std::string::npos;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '\'')
			inText = !inText;
		if (inText && name.at(p) == '\\') {
			p++;
			continue;
		}
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;
		
		if (!inText && openParentheses == 0 && name.at(p) == '|') {
			orPos = p;
			break;
		}
	}
	if (orPos != std::string::npos) {
		Automaton orAutomata(name);
		Node* start = orAutomata.createNode("startOA");
		Node* end = orAutomata.createNode("endOA");
		Automaton child0 = createAutomaton(lines, trim(name.substr(0, orPos)));
		Automaton child1 = createAutomaton(lines, trim(name.substr(orPos + 1)));
		
		orAutomata.link(EPSILON, start, &child0);
		orAutomata.link(EPSILON, start, &child1);
		orAutomata.link(EPSILON, &child0, end);
		orAutomata.link(EPSILON, &child1, end);
		
		orAutomata.addStartState(start);
		orAutomata.addEndState(end);
		
		return orAutomata;
	}
	
	auto concatenatePos = std::string::npos;
	openParentheses = 0;
	inText = false;
	for (unsigned long long int p = 0; p < name.length() - 1; p++) {
		if (name.at(p) == '\'')
			inText = !inText;
		if (inText && name.at(p) == '\\') {
			p++;
			continue;
		}
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;
		
		if (!inText && openParentheses == 0 && name.at(p) == ' ' && name.at(p + 1) != '|' && name.at(p + 1) != '&' && name.at(p + 1) != '*' && name.at(p + 1) != '+' && name.at(p + 1) != '?'
			&& p > 0 && name.at(p - 1) != '|' && name.at(p - 1) != '&') {
			concatenatePos = p;
			break;
		}
	}
	if (concatenatePos != std::string::npos) {
		Automaton concatenatedAutomaton(name);
		
		Node* start = concatenatedAutomaton.createNode("startCA");
		Node* end = concatenatedAutomaton.createNode("endCA");
		Automaton leftChild = createAutomaton(lines, trim(name.substr(0, concatenatePos)));
		Automaton rightChild = createAutomaton(lines, trim(name.substr(concatenatePos + 1)));
		
		concatenatedAutomaton.link(EPSILON, start, &leftChild);
		concatenatedAutomaton.link(EPSILON, &leftChild, &rightChild);
		concatenatedAutomaton.link(EPSILON, &rightChild, end);
		
		concatenatedAutomaton.addStartState(start);
		concatenatedAutomaton.addEndState(end);
		
		return concatenatedAutomaton;
	}
	
	auto unionPos = std::string::npos;
	openParentheses = 0;
	inText = false;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '\'')
			inText = !inText;
		if (inText && name.at(p) == '\\') {
			p++;
			continue;
		}
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;
		
		if (!inText && openParentheses == 0 && name.at(p) == '&') {
			unionPos = p;
			break;
		}
	}
	if (unionPos != std::string::npos) {
		Automaton unionAutomaton(name);
		Node* start = unionAutomaton.createNode("startUA");
		Node* end = unionAutomaton.createNode("endOA");
		Automaton child0 = createAutomaton(lines, trim(name.substr(0, unionPos)));
		Automaton child1 = createAutomaton(lines, trim(name.substr(unionPos + 1)));
		
		unionAutomaton.link(EPSILON, start, &child0);
		unionAutomaton.link(EPSILON, start, &child1);
		unionAutomaton.link(EPSILON, &child0, end);
		unionAutomaton.link(EPSILON, &child1, end);
		
		unionAutomaton.addStartState(start);
		unionAutomaton.addEndState(end);
		
		return unionAutomaton;
	}
	
	auto kleenePos = std::string::npos;
	openParentheses = 0;
	inText = false;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '\'')
			inText = !inText;
		if (inText && name.at(p) == '\\') {
			p++;
			continue;
		}
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;
		
		if (!inText && openParentheses == 0 && name.at(p) == '*') {
			kleenePos = p;
			break;
		}
	}
	if (kleenePos != std::string::npos) {
		Automaton loopingAutomaton(name);
		Node* hub = loopingAutomaton.createNode("hubLA");
		Automaton childAutomaton = createAutomaton(lines, trim(name.substr(0, kleenePos)));
		
		loopingAutomaton.link(EPSILON, hub, &childAutomaton);
		loopingAutomaton.link(EPSILON, &childAutomaton, hub);
		
		loopingAutomaton.addStartState(hub);
		loopingAutomaton.addEndState(hub);
		
		return loopingAutomaton;
	}
	
	auto plusPos = std::string::npos;
	openParentheses = 0;
	inText = false;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '\'')
			inText = !inText;
		if (inText && name.at(p) == '\\') {
			p++;
			continue;
		}
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;
		
		if (!inText && openParentheses == 0 && name.at(p) == '+') {
			plusPos = p;
			break;
		}
	}
	if (plusPos != std::string::npos) {
		Automaton atLeastOneAutomaton(name);
		Node* start = atLeastOneAutomaton.createNode("startALOA");
		Node* end = atLeastOneAutomaton.createNode("endALOA");
		Automaton childAutomaton = createAutomaton(lines, trim(name.substr(0, plusPos)));
		
		atLeastOneAutomaton.link(EPSILON, start, &childAutomaton);
		atLeastOneAutomaton.link(EPSILON, &childAutomaton, end);
		atLeastOneAutomaton.link(EPSILON, end, &childAutomaton);
		
		atLeastOneAutomaton.addStartState(start);
		atLeastOneAutomaton.addEndState(end);
		
		return atLeastOneAutomaton;
	}
	
	auto questionMarkPos = std::string::npos;
	openParentheses = 0;
	inText = false;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '\'')
			inText = !inText;
		if (inText && name.at(p) == '\\') {
			p++;
			continue;
		}
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;
		
		if (!inText && openParentheses == 0 && name.at(p) == '?') {
			questionMarkPos = p;
			break;
		}
	}
	if (questionMarkPos != std::string::npos) {
		Automaton perhapsAutomaton(name);
		Node* start = perhapsAutomaton.createNode("startPA");
		Node* end = perhapsAutomaton.createNode("endPA");
		Automaton childAutomaton = createAutomaton(lines, trim(name.substr(0, questionMarkPos)));
		
		perhapsAutomaton.link(EPSILON, start, end);
		perhapsAutomaton.link(EPSILON, start, &childAutomaton);
		perhapsAutomaton.link(EPSILON, &childAutomaton, end);
		
		perhapsAutomaton.addStartState(start);
		perhapsAutomaton.addEndState(end);
		
		return perhapsAutomaton;
	}
	
	if (name.at(0) == '(' && name.at(name.length() - 1) == ')') {
		return createAutomaton(lines, trim(name.substr(1, name.length() - 2)));
	}
	
	if (name.at(0) == '\'') {
		std::string text = reformatText(name.substr(1, name.length() - 2));
		Automaton textAutomaton(name);
		Node* start = textAutomaton.createNode("startTA");
		Node* lastNode = start;
		for (char& ch : text) {
			Node* charNode = textAutomaton.createNode(std::string(1, ch));
			textAutomaton.link(std::string(1, ch), lastNode, charNode);
			lastNode = charNode;
		}
		textAutomaton.addStartState(start);
		textAutomaton.addEndState(lastNode);
		return textAutomaton;
	} else {
		int index = 1;
		for (std::string line : lines) {
			line = trim(line);
			if (line[line.length() - 1] == ':' || line[line.length() - 1] == '>') {
				if (startsWith(line, name))
					break;
			}
			index++;
		}
		if (index >= lines.size()) {
			std::cout << "Couldn't find regex: \"" << name << "\"" << std::endl;
			return Automaton::nullAutomaton;
		}
		return createAutomaton(lines, shortenWhitespace(trim(lines[index])));
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

std::string shortenWhitespace(const std::string& s) {
	std::string res;
	bool inWhitespace = false;
	bool inText = false;
	
	for (int i = 0; i < s.length(); i++) {
		char ch = s[i];
		if (ch == '\'') {
			inText = !inText;
			if(inWhitespace)
				res += " ";
			inWhitespace = false;
		}
		if (inText) {
			res += ch;
			if (ch == '\\') {
				i++;
				res += s[i];
				continue;
			}
			continue;
		}
		
		if (ch == '\t' || ch == ' ')
			inWhitespace = true;
		else if (inWhitespace) {
			inWhitespace = false;
			res += " ";
			res += ch;
		} else {
			res += ch;
		}
	}
	
	return res;
}

std::string reformatText(const std::string& s) {
	std::string result = s;
	unsigned long long int replacePos = result.find("\\n");
	while (replacePos != std::string::npos) {
		result = result.replace(replacePos, 2, "\n");
		replacePos = result.find("\\n", replacePos);
	}
	
	replacePos = result.find("\\t");
	while (replacePos != std::string::npos) {
		result = result.replace(replacePos, 2, "\t");
		replacePos = result.find("\\t", replacePos);
	}
	
	replacePos = result.find("\\\\");
	while (replacePos != std::string::npos) {
		result = result.replace(replacePos, 2, "\\");
		replacePos = result.find("\\\\", replacePos);
	}

	replacePos = result.find("\\'");
	while (replacePos != std::string::npos) {
		result = result.replace(replacePos, 2, "'");
		replacePos = result.find("\\'", replacePos);
	}
	
	return result;
}

bool startsWith(const std::string& s, const std::string& test) {
	if (test.length() > s.length())
		return false;
	for (int i = 0; i < test.length(); i++) {
		if (s[i] != test[i])
			return false;
	}
	return true;
}