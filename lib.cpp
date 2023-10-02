// lib.cpp
//
// Author: Noah BEAUFILS
// Date: 2-oct-2023

#include "irc.hpp"

int	_stoi(std::string const &str) {

	if (str.size() > 10)
		throw std::length_error("number too long");

	int		val = 0;
	bool	isPositive = true;
	std::string::const_iterator	it;
	it = str.begin();

	while (isspace(*it)) { ++it; } // skip white space
	if (*it == '-') {
		isPositive = false;
		++it;
	}
	else if (*it == '+')
		++it;
	while (isdigit(*it))
		val = val * 10 + (*it++) - '0';
	return (isPositive ? val : -val);
}