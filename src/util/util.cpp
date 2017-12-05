//
// Created by Ole on 05.12.2017.
//

#include "util.h"

namespace util {
	
	std::string trim(const std::string& s) {
		if (s.empty())
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
	
	std::string shortenWhitespace(const std::string& s) {
		std::string res;
		bool inWhitespace = false;
		bool inText = false;
		
		for (int i = 0; i < s.length(); i++) {
			char ch = s[i];
			if (ch == '\'') {
				inText = !inText;
				if (inWhitespace)
					res += " ";
				inWhitespace = false;
			}
			if (inText) {
				res += ch;
				if (ch == '\\') {
					i++;
					res += s[i];
					continue;
				}
				continue;
			}
			
			if (ch == '\t' || ch == ' ')
				inWhitespace = true;
			else if (inWhitespace) {
				inWhitespace = false;
				res += " ";
				res += ch;
			} else {
				res += ch;
			}
		}
		
		return res;
	}
	
	std::string reformatText(const std::string& s) {
		std::string result = s;
		unsigned long long int replacePos = result.find("\\n");
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
		
		replacePos = result.find("\\'");
		while (replacePos != std::string::npos) {
			result = result.replace(replacePos, 2, "'");
			replacePos = result.find("\\'", replacePos);
		}
		
		return result;
	}
	
	bool startsWith(const std::string& s, const std::string& test) {
		if (test.length() > s.length())
			return false;
		for (int i = 0; i < test.length(); i++) {
			if (s[i] != test[i])
				return false;
		}
		return true;
	}
	
}
