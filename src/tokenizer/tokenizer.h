//
// Created by Ole on 30.11.2017.
//

#pragma once

#include <vector>
#include <string>
#include "token.h"
#include "regex.h"

class Tokenizer {
private:
	std::vector<Token> tokens;
	Regex regex;
public:
	Tokenizer(const std::string& regexFile);

	void tokenize(const std::string& text);

	const std::vector<Token>& getTokens() {
		return tokens;
	}
};


