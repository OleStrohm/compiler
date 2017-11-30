//
// Created by Ole on 30.11.2017.
//

#pragma once

#include <string>

struct Token {
	const std::string type;
	std::string text;

	Token(std::string type, std::string text)
			: type(std::move(type)), text(std::move(text)) {}
};
