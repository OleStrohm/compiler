//
// Created by Ole on 25.11.2017.
//

#pragma once

#include <string>
#include <utility>
#include "automata.h"
#include "token.h"

class Regex {
private:
	std::vector<std::string> names;
	std::vector<Automaton> automata;
public:
	explicit Regex(const std::string& path);
	
	Token extractToken(const std::string& text, unsigned int start, unsigned int* end);
};
