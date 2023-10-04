// server.cpp
//
// Author: Noah BEAUFILS
// Date: 4-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
int const &Server::getFd( void ) const { return _fd; }
std::map<int, Client *> const &Server::getClients( void ) const { return _clients; }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Server::Server(int port, std::string const &password) : _password(password) {

	/* create a socket (IPV4, TCP) */
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
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

	if (listen(this->_fd, MAX_CONNECTION) < 0)
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
