#include <iostream>
#include <ctime>
#include "tokenizer/tokenizer.h"

std::string visualizeWhitespace(const std::string &s);

int main() {
	std::string text = "float x = -.5f\n"
			           "			int potet    = +.7e37;";
	std::cout << std::endl;
	std::cout << text << std::endl;
	std::cout << std::endl;

	Tokenizer tokenizer("tokenizer_regex");
	auto time0 = clock();

	tokenizer.tokenize(text);
	for (auto token : tokenizer.getTokens())
		printf("%-11s  |  %s\n", token.type.c_str(), visualizeWhitespace(token.text).c_str());

	std::cout << std::endl << "Time used: " << (clock() - time0) / (float) CLOCKS_PER_SEC << "ms" << std::endl;

	return 0;
}

std::string visualizeWhitespace(const std::string &s) {
	std::string res;
	for (char ch : s) {
		if (ch == ' ') {
			res += "\\s";
		} else if (ch == '\n') {
			res += "\\n";
		} else if (ch == '\t') {
			res += "\\t";
		} else {
			res += ch;
		}
	}
	return res;
}
