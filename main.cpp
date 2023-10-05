// main.cpp
//
// Author: Noah BEAUFILS
// Date: 4-oct-2023

#include "irc.hpp"

Server *Server_ptr = NULL;

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
	catch (Server::init_error &error) {
		std::cerr << "error: " << error.what() << std::endl;
		return 1;
	}
	catch (Server::run_error &error) {
		std::cerr << "error: " << error.what() << std::endl;
		delete Server_ptr;
		return 1;
	}

	delete Server_ptr;
	return 0;
}

// sources:
/* [ "https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/" ] */
/* [ "https://www.cs.cmu.edu/~srini/15-441/S10/project1/pj1_description.pdf" ] */
/* [ "http://vidalc.chez.com/lf/socket.html#lowlevel" ] */
/* [ "https://www.rfc-editor.org/rfc/rfc2812" ] */