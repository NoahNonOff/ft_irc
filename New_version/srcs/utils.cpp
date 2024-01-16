// utils.cpp
//
// Author: Noah BEAUFILS
// Date: 8-jan-2024

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

/* split the command */
std::vector<std::string>	splitCmds(std::string const command) {

	int	i = 0, end = 0;
	std::vector<std::string>	ret;

	if (command.size() < 1)
		return ret;
	for (; i < (int)command.size() && command[i] == ' '; i++)
		;;
	while (1) {
		for (end = i; end < (int)command.size() && command[end] != ' '; end++)
			;;
		if (command[i] == ':')
			end = command.size();
		ret.push_back(command.substr(i, end - i));
		if (!(end < (int)command.size()))
			break ;
		for (i = end; i < (int)command.size() && command[i] == ' '; i++)
			;;
		if (!(i < (int)command.size()))
			break ;
	}
	return ret;
}