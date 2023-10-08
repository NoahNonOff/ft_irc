// command.cpp
//
// Author: Noah BEAUFILS
// Date: 8-oct-2023

#include "irc.hpp"

bool	is_request(std::string const &request) {

	int	i = 0;
	for (; i < (int)request.size() && request[i] == ' '; i++)
		;;
	return request[i] == '/' ? true : false;
}

std::vector<std::string>	splitCmds(std::string const command) {

	int	i = 0, end = 0;
	std::vector<std::string>	ret;

	if (command.size() < 1)
		return ret;
	for (; i < (int)command.size() && command[i] == ' '; i++)
		;;
	i++; // pass the '/'
	while (1) {
		for (end = i; end < (int)command.size() && command[end] != ' '; end++)
			;;
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