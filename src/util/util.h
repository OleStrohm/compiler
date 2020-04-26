//
// Created by Ole on 05.12.2017.
//

#pragma once

#include <string>
#include <sstream>

namespace util {
	std::string trim(const std::string& s);
	std::string shortenWhitespace(const std::string& s);
	std::string reformatText(const std::string& s);
	bool startsWith(const std::string& s, const std::string& test);
	bool isWhitespace(const unsigned char&);
	bool isLetter(const unsigned char&);
	bool isStringStreamEmpty(const std::stringstream& ss);
}
