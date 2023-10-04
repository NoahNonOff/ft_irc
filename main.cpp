// main.cpp
//
// Author: Noah BEAUFILS
// Date: 4-oct-2023

#include "irc.hpp"

Server *server;

/* ---------------------------- Main-loop ------------------------------ */
void	main_loop(void) {

	std::cout << "waiting for connections ..." << std::endl;
	while (1) {
		// to do
	}
}

/* ------------------------------- main -------------------------------- */
void	endProg(int signal) {

	if (signal == SIGINT) {
		std::cout << "\033[2D";
		delete server;
		exit (0);
	}
}

int	main(int ac, char *av[])
{
	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 2;
	}

	/* initiate the server */
	try {
		server = new Server(_stoi(av[1]), av[2]);
	}
	catch (std::exception &error) {
		std::cerr << "error: " << error.what() << std::endl;
		return 1;
	}

	/* initialize the signal handler to close the server */
	std::signal(SIGINT, &endProg);

	/* wait for client */
	main_loop();

	delete server;
	return 0;
}

/* [ "https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/" ] */
/* [ "http://vidalc.chez.com/lf/socket.html#lowlevel" ] */