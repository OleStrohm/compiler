#include <iostream>
#include <ctime>
#include "tokenizer/tokenizer.h"
#include "syntax/contextFreeGrammar.h"
#include "util/util.h"
#include "syntax/CFGTree.h"
#include "syntax/ast.h"

std::string visualizeWhitespaceAndFormat(const std::string &s);

#define TEST_TOKENIZER 0
#define TEST_SYNTAXIZER 1

int main() {
#if TEST_TOKENIZER
	std::string text = "float x = -.5f;\n"
					   "string potato = getPotato(2);";
	std::cout << std::endl;
	std::cout << text << std::endl;
	std::cout << std::endl;

	Tokenizer tokenizer("tokenizer_regex");
	auto time0 = clock();

	tokenizer.tokenize(text);
	for (const auto& token : tokenizer.getTokens())
		printf("%-11s  |  %s\n", token.type.c_str(), visualizeWhitespaceAndFormat(token.text).c_str());

	std::cout << std::endl << "Time used: " << (clock() - time0) / (float) CLOCKS_PER_SEC << "ms" << std::endl;
#endif

#if TEST_SYNTAXIZER
	std::string source = "if(id - num) { id = num; }";
//	std::string source = "id = (num + id) * num;";

	CFG grammar("context_free.grammar");
	std::vector<Expansion> stacktrace;
	bool res = grammar.validate(source, stacktrace);
	std::cout << "Validation: " << (res ? "PASSED" : "FAILED") << std::endl;

	if(!res)
		return 0;

	std::vector<Expansion> stacktrace_rev = util::reverse<Expansion>(stacktrace);
	if (!stacktrace_rev.empty()) {
		std::cout << "Stack trace:" << std::endl;
		for (auto &e : stacktrace_rev) {
			std::cout << "\t" << e << std::endl;
		}
	}

	std::cout << std::endl << "CFG parse tree:" << std::endl;
	CFGTree *parseTree = CFGTree::generateTree(stacktrace_rev);
	TreeDisplay display = parseTree->printTree();
	display.show();

	AST *ast = AST::generateAST(parseTree);
	std::cout << std::endl << "AST:" << std::endl;
	ast->printTree().show();
	std::cout << std::endl;

	std::cout << "\tSource code: '" << source << "'" << std::endl;
	std::cout << "Compiled code:" << std::endl;
	std::cout << ast->code() << std::endl;

#endif

	return 0;
}

std::string visualizeWhitespaceAndFormat(const std::string &s) {
	std::string res;
	bool isStringLiteral = s[0] == '"';
	bool foundBackslash = false;
	for (char ch : s) {
		if (ch == ' ' && !isStringLiteral) {
			res += "\\s";
			foundBackslash = false;
		} else if (ch == '\n' && !isStringLiteral) {
			res += "\\n";
			foundBackslash = false;
		} else if (ch == '\t' && !isStringLiteral) {
			res += "\\t";
			foundBackslash = false;
		} else if (ch == '\\' && isStringLiteral && !foundBackslash) {
			foundBackslash = true;
			continue;
		} else {
			res += ch;
			foundBackslash = false;
		}
	}
	return res;
}
