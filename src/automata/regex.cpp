//
// Created by Ole on 25.11.2017.
//

#include <fstream>
#include <vector>
#include <iostream>
#include "regex.h"
#include "automata.h"

#define LOG(x) std::cout << x << std::endl;

Automata createAutomaton(const std::vector<std::string> &lines, const std::string &name);
std::string trim(const std::string &s);
std::string shortenWhitespace(const std::string &s);
std::string reformatText(const std::string &s);
bool startsWith(const std::string &s, const std::string &test);

Regex::Regex(const std::string &path) {
	std::ifstream file(path);
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		line = trim(line);
		const std::string& lastLine = lines[lines.size() - 1];
		if (line.empty()) {
			continue;
		} else if (!lines.empty() && !(line[line.length() - 1] == '>' || line[line.length() - 1] == ':') && !(lines[lines.size() - 1][lines[lines.size() - 1].length() - 1] == '>' || lines[lines.size() - 1][lines[lines.size() - 1].length() - 1] == ':')) {
			lines[lines.size() - 1] = lines[lines.size() - 1] + " | " + line;
		} else {
			lines.emplace_back(line);
		}
	}

//	for (const std::string &line : lines) {
//		std::cout << line << std::endl;
//	}
//	std::cout << std::endl;

	Automata a = createAutomaton(lines, shortenWhitespace(trim(lines[1])));
	LOG("finished");
}

Automata createAutomaton(const std::vector<std::string> &lines, const std::string &name) {
	LOG("Parsing: \"" << name << "\"");
	int openParentheses = 0;
	bool inText = false;

	auto orPos = std::string::npos;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '.')
			inText = true;
		if (name.at(p) == ' ')
			inText = false;
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;

		if (!inText && openParentheses == 0 && name.at(p) == '|') {
			orPos = p;
			break;
		}
	}
	if (orPos != std::string::npos) { // Todo: Do you want this or that?
		Automata orAutomata(name);
		Node* start = orAutomata.createNode("startOA");
		Node* end = orAutomata.createNode("endOA");
		Automata child0 = createAutomaton(lines, trim(name.substr(0, orPos)));
		Automata child1 = createAutomaton(lines, trim(name.substr(orPos + 1)));

		orAutomata.link(EPSILON, start, &child0);
		orAutomata.link(EPSILON, start, &child1);
		orAutomata.link(EPSILON, &child0, end);
		orAutomata.link(EPSILON, &child1, end);
		
		LOG("or-ing two things");

		return orAutomata;
	}

	auto concatenatePos = std::string::npos;
	openParentheses = 0;
	inText = false;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '.')
			inText = true;
		if (name.at(p) == ' ')
			inText = false;
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;

		if (openParentheses == 0 && name.at(p) == ' ' && name.at(p + 1) != '|' && name.at(p + 1) != '&' && name.at(p + 1) != '*' && name.at(p + 1) != '+' && name.at(p + 1) != '?'
			&& p > 0 && name.at(p - 1) != '|' && name.at(p - 1) != '&') {
			concatenatePos = p;
			break;
		}
	}
	if (concatenatePos != std::string::npos) { // Todo: concatenate everything
		Automata concatenatedAutomaton(name);

		Node* start = concatenatedAutomaton.createNode("startCA");
		Node* end = concatenatedAutomaton.createNode("endCA");
		Automata leftChild = createAutomaton(lines, trim(name.substr(0, concatenatePos)));
		Automata rightChild = createAutomaton(lines, trim(name.substr(concatenatePos + 1)));

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
		if (name.at(p) == '.')
			inText = true;
		if (name.at(p) == ' ')
			inText = false;
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;

		if (!inText && openParentheses == 0 && name.at(p) == '&') {
			unionPos = p;
			break;
		}
	}
	if (unionPos != std::string::npos) { // Todo: union these sons of bitches
		Automata unionAutomaton(name);
		Node* start = unionAutomaton.createNode("startUA");
		Node* end = unionAutomaton.createNode("endOA");
		Automata child0 = createAutomaton(lines, trim(name.substr(0, unionPos)));
		Automata child1 = createAutomaton(lines, trim(name.substr(unionPos + 1)));

		unionAutomaton.link(EPSILON, start, &child0);
		unionAutomaton.link(EPSILON, start, &child1);
		unionAutomaton.link(EPSILON, &child0, end);
		unionAutomaton.link(EPSILON, &child1, end);

		return unionAutomaton;
	}

	auto kleenePos = std::string::npos;
	openParentheses = 0;
	inText = false;
	for (unsigned long long int p = 0; p < name.length(); p++) {
		if (name.at(p) == '.')
			inText = true;
		if (name.at(p) == ' ')
			inText = false;
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;

		if (!inText && openParentheses == 0 && name.at(p) == '*') {
			kleenePos = p;
			break;
		}
	}
	if (kleenePos != std::string::npos) { // Todo: LOOP THE FUCKING LOOP
		Automata loopingAutomaton(name);
		Node* hub = loopingAutomaton.createNode("hubLA");
		Automata childAutomaton = createAutomaton(lines, trim(name.substr(0, kleenePos)));
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
		if (name.at(p) == '.')
			inText = true;
		if (name.at(p) == ' ')
			inText = false;
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;

		if (!inText && openParentheses == 0 && name.at(p) == '+') {
			plusPos = p;
			break;
		}
	}
	if (plusPos != std::string::npos) { // Todo: ONCE + LOOP THE FUCKING LOOP
		Automata atLeastOneAutomaton(name);
		Node* start = atLeastOneAutomaton.createNode("startALOA");
		Node* end = atLeastOneAutomaton.createNode("endALOA");
		Automata childAutomaton = createAutomaton(lines, trim(name.substr(0, plusPos)));
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
		if (name.at(p) == '.')
			inText = true;
		if (name.at(p) == ' ')
			inText = false;
		if (!inText && name.at(p) == '(')
			openParentheses++;
		if (!inText && name.at(p) == ')')
			openParentheses--;

		if (!inText && openParentheses == 0 && name.at(p) == '?') {
			questionMarkPos = p;
			break;
		}
	}
	if (questionMarkPos != std::string::npos) { // Todo: YAY OR NAY
		Automata perhapsAutomaton(name);
		Node* start = perhapsAutomaton.createNode("startPA");
		Node* end = perhapsAutomaton.createNode("endPA");
		Automata childAutomaton = createAutomaton(lines, trim(name.substr(0, questionMarkPos)));
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

	if (name.at(0) == '.') { // todo: create text Automaton.
		std::string text = reformatText(name.substr(1));
		Automata textAutomaton(name);
		Node* start = textAutomaton.createNode("startTA");
		Node* lastNode = start;
		for (char ch : text) {
			Node* charNode = textAutomaton.createNode("" + ch);
			textAutomaton.link("" + ch, lastNode, charNode);
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
			return Automata::nullAutomata;
		}
		return createAutomaton(lines, shortenWhitespace(trim(lines[index])));
	}
}

std::string trim(const std::string &s) {
	if(s.empty())
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

std::string shortenWhitespace(const std::string &s) {
	std::string res;
	bool inWhitespace = false;

	for (char c : s) {
		if (c == '\t' || c == ' ')
			inWhitespace = true;
		else if (inWhitespace) {
			inWhitespace = false;
			res += " ";
			res += c;
		} else {
			res += c;
		}
	}

	return res;
}

std::string reformatText(const std::string &s) {
	std::string result = s;
	unsigned long long int replacePos = result.find("\\s");
	while (replacePos != std::string::npos) {
		result = result.replace(replacePos, 2, " ");
		replacePos = result.find("\\s", replacePos);
	}

	replacePos = result.find("\\n");
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
	return result;
}

bool startsWith(const std::string &s, const std::string &test) {
	if (test.length() > s.length())
		return false;
	for (int i = 0; i < test.length(); i++) {
		if (s[i] != test[i])
			return false;
	}
	return true;
}
