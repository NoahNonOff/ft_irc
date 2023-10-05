// lib.cpp
//
// Author: Noah BEAUFILS
// Date: 5-oct-2023

#include "irc.hpp"

/* convert a string in an int */ 
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

/* convert a dirty string (with '\n', '\r', ...) to a clean string */
std::string	_mtos(char *msg) {

	int			end;
	std::string	ret = msg;

	if (ret.size() < 1)
		return ret;
	for (end = 0; ret[end] && ret[end] != '\r'; end++)
		;;
	ret.erase(end);
	for (end = 0; ret[end] && ret[end] != '\n'; end++)
		;;
	ret.erase(end);
	return ret;
}

/* initialize the t_roles structure */
t_roles	createT_roles(bool admin) {

	t_roles	ret;

	ret.admin = admin;
	return ret;
}