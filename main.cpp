// main.cpp
//
// Author: Noah BEAUFILS
// Date: 4-oct-2023

#include "irc.hpp"

Server *Server_ptr;

void	endProg(int signal) {

	if (signal == SIGINT) {
		std::cout << "\033[2D"; /* remove the ^C */
		delete Server_ptr;
		exit (0);
	}
}

int	main(int ac, char *av[])
{
	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 2;
	}

	try {
		Server_ptr = new Server(_stoi(av[1]), av[2]);

		/* initialize the signal handler to close the server */
		std::signal(SIGINT, &endProg);

		/* run the server and wait for client */
		Server_ptr->run();
	}
	catch (std::exception &error) {
		std::cerr << "error: " << error.what() << std::endl;
		return 1;
	}

	delete Server_ptr;
	return 0;
}

/* [ "https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/" ] */
/* [ "http://vidalc.chez.com/lf/socket.html#lowlevel" ] */