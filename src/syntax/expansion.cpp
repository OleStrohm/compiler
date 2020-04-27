//
// Created by ole on 27/04/2020.
//

#include "expansion.h"
#include <sstream>
#include "../util/util.h"

std::string concatExpansion(const Expansion &expansion) {
	std::stringstream ss;
	for (auto &s : expansion.symbols) {
		ss << s << " ";
	}
	std::string res = ss.str();
	return std::move(res.substr(0, res.length() - 1));
}

std::string concatExpansion(const std::vector<std::string> &expansion) {
	std::stringstream ss;
	for (auto &s : expansion) {
		ss << s << "|";
	}
	std::string res = ss.str();
	return std::move(res.substr(0, res.length() - 1));
}

std::ostream &operator<<(std::ostream &stream, const Expansion &e) {
	stream << e.symbol << " => " << concatExpansion(e);
	return stream;
}

Expansion::Expansion(std::string symbol, const std::string &expansionString) {
	this->symbol = std::move(symbol);
	std::stringstream ss;
	for (char c : expansionString) {
		if (util::isWhitespace(c)) {
			if (!util::isStringStreamEmpty(ss)) {
				symbols.push_back(std::move(ss.str())); // TODO: ??
				ss.str("");
			}
			continue;
		}
		if (util::isLetter(c)) {
			ss << c;
			continue;
		}
		if (!util::isStringStreamEmpty(ss)) {
			symbols.push_back(std::move(ss.str())); // TODO: ??
			ss.str("");
		}
		ss << c;
		symbols.push_back(std::move(ss.str())); // TODO: ??
		ss.str("");
	}
	if (!util::isStringStreamEmpty(ss)) {
		symbols.push_back(std::move(ss.str())); // TODO: ??
		ss.flush();
	}
}