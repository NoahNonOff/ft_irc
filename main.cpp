// main.cpp
//
// Author: Noah BEAUFILS
// Date: 2-oct-2023

#include "irc.hpp"

/* [ "https://ncona.com/2019/04/building-a-simple-server-with-cpp/" ] */
/* [ "https://github.com/irssi/irssi" ] */

int	server_fd = -1;

/* ------------------------ initiate the server ------------------------ */
void	server_init(int port) {

	/* create a socket (IPV4, TCP) */
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == server_fd)
		throw std::runtime_error("failed to create a socket");

	int	opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");

	/* binding to the port */
	/* ["https://learn.microsoft.com/en-us/windows/win32/api/ws2def/ns-ws2def-sockaddr_in"] */
	sockaddr_in	sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(port); /* htons convert number to network byte order */

	if (bind(server_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
		throw std::runtime_error("failed during binding");

	/* start listening and set the number of possible connections */
	if (listen(server_fd, MAX_CONNECTION) < 0)
		throw std::runtime_error("failed to listen on socket");	
}

/* -------------------------- Accept-Client ---------------------------- */
void	acceptClient() {

	int					client_fd = -1;
	struct sockaddr_in	client_addr;
	socklen_t			addr_len = sizeof(client_addr);

	/* accept the client connection */
	if ((client_fd = accept((server_fd), (struct sockaddr *)&client_addr, &addr_len)) < 0) {
		std::cerr << "error: failed during the acceptation" << std::endl;
		return ;
	}

	/* create the argument for the thread */
	Client	*arg = NULL;
	if (!(arg = new Client(client_fd))) {
		close(client_fd);
		std::cerr << "error: allocation failed" << std::endl;
		return ;
	}

	*arg = client_fd;

	/* create the thread to handle client request */
	pthread_t	thrID;
	pthread_create(&thrID, NULL, handle_client, (void *)arg);
	pthread_detach(thrID);
}

/* ------------------------------- main -------------------------------- */
void	endProg(int signal) {

	if (signal == SIGINT) {
		std::cout << "\033[2DServer closed" << std::endl;
		close(server_fd);
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
		server_init(_stoi(av[1]));
	}
	catch (std::exception &error) {
		std::cerr << "error: " << error.what() << std::endl;
		return 1;
	}

	/* initialize the signal handler to close the server */
	std::signal(SIGINT, &endProg);

	/* wait for client */
	while (1)
		acceptClient();

	close(server_fd);
	return 0;
}