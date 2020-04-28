//
// Created by ole on 27/04/2020.
//

#pragma once

#include <vector>
#include <string>

class TreeDisplay {
	std::vector<std::vector<std::string>> screen;
	std::vector<std::string> paddings;

public:

	void printString(int x, int y, const std::string &s);

	void show();
};