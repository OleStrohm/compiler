//
// Created by Ole on 30.11.2017.
//

#include "tokenizer.h"
#include <iostream>

Tokenizer::Tokenizer(const std::string& regexFile)
	: regex(regexFile + ".reg") {}

void Tokenizer::tokenize(const std::string& text) {
	unsigned int end = 0;
	while (end < text.length()) {
		Token token = regex.extractToken(text, end, &end);
		if (token.type != "error" || tokens.empty() || tokens[tokens.size() - 1].type != "error")
			tokens.push_back(token);
		else
			tokens[tokens.size() - 1].text += token.text;
	}
}