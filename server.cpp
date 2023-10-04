// server.cpp
//
// Author: Noah BEAUFILS
// Date: 4-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
int const &Server::getFd( void ) const { return _fd; }
std::string const &Server::getPassword(void) const { return _password; };
std::map<int, Client *> const &Server::getClients( void ) const { return _clients; }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Server::Server(int port, std::string const &password) : _password(password) {

	/* create a socket (IPV4, TCP) */
	this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (-1 == this->_fd)
		throw std::runtime_error("failed to create a socket");

	/* remove a binding error (bind even if the port is already use) */
	int	opt = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");

	/* binding to the port */
	this->_sockAddr.sin_family = AF_INET;
	this->_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY); // use my IP address
	this->_sockAddr.sin_port = htons(port); // htons convert number to network byte order

	if (bind(this->_fd, (struct sockaddr *)&this->_sockAddr, sizeof(sockaddr)) < 0)
		throw std::runtime_error("failed during binding");

	if (listen(this->_fd, MAX_WAIT) < 0)
		throw std::runtime_error("failed to listen on socket");

	std::cout << "listen on port " << port << std::endl;
}

Server::~Server() {

	/* destroy the server */
	std::map<int, Client *>::iterator	it = _clients.begin();

	for (; it != _clients.end(); ++it)
		delete it->second;

	_clients.clear();
	close(_fd);
	std::cout << "Server closed" << std::endl;
}

/* ----------------------------------- functions ----------------------------------- */

void	Server::run(void) {

	int		max_sd, activity;

	std::cout << "waiting for connections ..." << std::endl;
	while (1) {

		FD_ZERO(&this->_readfds); // clear socket_set
		FD_SET(this->_fd, &this->_readfds); // add server ID to socket_set
		max_sd = this->_fd;

		/* add all client ID to socket_set */
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {

			int	sd = it->second->getFd();
			if (sd > 0)
				FD_SET(sd, &this->_readfds);
			if (sd > max_sd)
				max_sd = sd;
		}

		activity = select(max_sd + 1, &this->_readfds, NULL, NULL, NULL);
		if (activity < 0 && errno != EINTR)
			throw std::runtime_error("select failed");

		/* incoming connection */
		if (FD_ISSET(this->_fd, &this->_readfds))
			this->addClient();

		/* I/O operation from client */
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			int	sd = it->first;

			if (FD_ISSET(sd, &_readfds))
				this->clientRequest(sd);
		}
	}
}

void	Server::clientRequest(int sd) {

	ssize_t	b_read = -1;
	char	buff[BUFFER_SIZE] = {0};

	b_read = recv(sd, buff, BUFFER_SIZE, 0);

	/* destroy the client */
	if (b_read < 1) {
		delete _clients.find(sd)->second;
		_clients.erase(_clients.find(sd));
	}
	else {
		buff[b_read - TRASH_SIZE] = 0; // remove the gliberish ('\n', '\r', ...)
		std::string	msg = buff;
		std::cout << _clients.find(sd)->second->getUsername() << ": {" << msg << "}" << std::endl;
	}
}

void	Server::addClient(void) {

	std::cout << "someone wants to connect to the server" << std::endl;

	/* accept the client */
	socklen_t	addr_len = sizeof(_sockAddr);
	int	new_socket = accept(_fd, (struct sockaddr *)&_sockAddr, &addr_len);

	if (new_socket < 0)
		throw std::runtime_error("failed during acceptation");

	/* add the new socket to the socket_set */
	this->_clients[new_socket] = new Client(new_socket);
}