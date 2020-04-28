//
// Created by ole on 27/04/2020.
//

#include "treedisplay.h"

#include <iostream>

void TreeDisplay::printString(int x, int y, const std::string &s) {
	while (screen.size() < y + 1) {
//		if(screen.empty())
		screen.emplace_back();
//		else
//			screen.push_back(screen[screen.size()-1]);
	}
	std::vector<std::string> &row = screen[y];
	if (row.size() < x + 1) {
		for (auto &row2 : screen)
			while (row2.size() < x + 1) {
				row2.emplace_back();
				paddings.emplace_back();
			}
	}
	std::string &padding = paddings[x];
	while (padding.length() < s.length())
		padding += ' ';
	//while(y < screen.size())
	screen[y][x] = s;
}

void TreeDisplay::show() {
	for (int y = 0; y < screen.size(); y++) {
		auto &row = screen[y];
		std::cout << "| ";
		for (int x = 0; x < row.size(); x++) {
			const std::string &s = row[x];
			int padding = paddings[x].length() - s.length();
			x++;
			while (x < row.size() && row[x].empty()) {
				bool columnEmptyAbove = true;
				for (int ty = y; ty >= 0; ty--) {
					if (!screen[ty][x].empty()) {
						columnEmptyAbove = false;
						break;
					}
				}
				if (!columnEmptyAbove) {
					break;
				}
				padding += paddings[x].length() + 3;
				x++;
			}
			x--;
			int halfPadding = padding / 2;
			std::string newPadding;
			while (newPadding.length() < halfPadding)
				newPadding += ' ';
			std::cout << newPadding << s << newPadding << (halfPadding * 2 < padding ? " " : "") << (s.empty() && (x == row.size() || row[x+1].empty()) ? "   " : " | ");
		}
		std::cout << std::endl;
	}
}