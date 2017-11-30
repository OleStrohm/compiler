#include <iostream>
#include "automata/automata.h"
#include "automata/regex.h"

std::string visualizeWhitespace(const std::string& s);

int main() {
	Regex reg("regexExample.txt");
	
	std::string text = "float x = -.5f;";
	std::cout << std::endl;
	std::cout << text << std::endl;
	std::cout << std::endl;
	
	unsigned int end = 0;
	while (end < text.length()) {
		Token s = reg.extractToken(text, end, &end);
		printf("%-11s  |  %s\n", s.type.c_str(), visualizeWhitespace(s.text).c_str());
	}
	
	return 0;
}

std::string visualizeWhitespace(const std::string& s) {
	std::string res;
	for(char ch : s) {
		if(ch == ' ') {
			res += "\\s";
		} else if(ch == '\n') {
			res += "\\n";
		} else if(ch == '\t') {
			res += "\\t";
		} else {
			res += ch;
		}
	}
	return res;
}
