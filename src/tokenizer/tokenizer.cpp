//
// Created by Ole on 30.11.2017.
//

#include "tokenizer.h"

Tokenizer::Tokenizer(std::string regexFile)
	: regex(regexFile) {}

void Tokenizer::tokenize(std::string text) {
	unsigned int end = 0;
	while (end < text.length()) {
		Token token = regex.extractToken(text, end, &end);
		if (token.type != "error" || tokens[tokens.size() - 1].type != "error")
			tokens.push_back(token);
		else
			tokens[tokens.size() - 1].text += token.text;
	}
}