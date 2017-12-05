#include <iostream>
#include <ctime>
#include "tokenizer/tokenizer.h"
#include "syntax/CFG.h"

std::string visualizeWhitespaceAndFormat(const std::string& s);

#define TEST_TOKENIZER 0
#define TEST_SYNTAXIZER 1

int main() {
#if TEST_TOKENIZER
	std::string text = "float x = -.5f\n"
			"			string potet    = \"quote: \\\"I have a dream!\\\"\";";
	std::cout << std::endl;
	std::cout << text << std::endl;
	std::cout << std::endl;
	
	Tokenizer tokenizer("tokenizer_regex");
	auto time0 = clock();
	
	tokenizer.tokenize(text);
	for (auto token : tokenizer.getTokens())
		printf("%-11s  |  %s\n", token.type.c_str(), visualizeWhitespaceAndFormat(token.text).c_str());
	
	std::cout << std::endl << "Time used: " << (clock() - time0) / (float) CLOCKS_PER_SEC << "ms" << std::endl;
#endif

#if TEST_SYNTAXIZER
	CFG contextFreeGrammar("context_free.grammar");



#endif
	
	return 0;
}

std::string visualizeWhitespaceAndFormat(const std::string& s) {
	std::string res;
	bool stringliteral = s[0] == '"';
	bool foundBackslash = false;
	for (char ch : s) {
		if (ch == ' ' && !stringliteral) {
			res += "\\s";
			foundBackslash = false;
		} else if (ch == '\n' && !stringliteral) {
			res += "\\n";
			foundBackslash = false;
		} else if (ch == '\t' && !stringliteral) {
			res += "\\t";
			foundBackslash = false;
		} else if (ch == '\\' && stringliteral && !foundBackslash) {
			foundBackslash = true;
			continue;
		} else {
			res += ch;
			foundBackslash = false;
		}
	}
	return res;
}
