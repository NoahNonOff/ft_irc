// clt_main.cpp
//
// Author: Noah BEAUFILS
// Date: 10-oct-2023

#include "clt.hpp"

Clt	*clt = NULL;

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

void	endProg(int signal) {

	if (signal == SIGINT) {
		std::cout << "\033[uconnection closed by foreigned host" << std::endl;
		delete clt;
		exit (0);
	}
}

int main(int ac, char **av) {

	int	ret = 0;
	if (ac < 2 || ac > 3) {
		std::cerr << "Usage: " << av[0] << " (<host>) <port>" << std::endl;
		return 1;
	}

	try {
		/* initiate the client */
		std::signal(SIGINT, &endProg);
		std::string	host = ac==2 ? "127.0.0.1" : av[1];
		int			port = ac==2 ? _stoi(av[1]) : _stoi(av[2]);
		clt = new Clt(host, port);

		/* initialize the signal handler to close the client */

		/* run the client */
		clt->clientLoop();
	}
	catch (std::exception &error) {
		std::cerr << "error: " << error.what() << std::endl;
		ret = 1;
	}

	/* destroy the client */
	delete clt;
	return ret;
}