//
// Created by ole on 27/04/2020.
//

#pragma once

#include <iostream>
#include <vector>

struct Expansion {
	std::string symbol;
	std::vector<std::string> symbols;

	explicit Expansion(std::string symbol, const std::string&);
};

std::string concatExpansion(const Expansion &expansion);
std::string concatExpansion(const std::vector<std::string> &expansion);
std::ostream& operator<<(std::ostream& stream, const Expansion& e);