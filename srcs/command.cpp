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