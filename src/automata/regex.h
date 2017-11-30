//
// Created by Ole on 25.11.2017.
//

#pragma once
#include <string>
#include <utility>
#include "automata.h"

struct Token {
	const std::string& type;
	const std::string text;
	
	Token(const std::string& type, std::string text)
			: type(type), text(std::move(text)) {}
};

class Regex {
private:
	std::vector<std::string> names;
	std::vector<Automaton> automata;
public:
	explicit Regex(const std::string& path);
	
	Token extractToken(const std::string& text, unsigned int start, unsigned int* end);
};
