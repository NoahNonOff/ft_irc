// server.cpp
//
// Author: Noah BEAUFILS
// Date: 12-oct-2023

#include "irc.hpp"


/* ---------------------------------- Set and Get ---------------------------------- */
int const &Server::getFd( void ) const { return _fd; }
std::string const &Server::getPassword(void) const { return _password; };
std::map<int, Client *> const &Server::getClients( void ) const { return _clients; }
std::vector<Channel *> const &Server::getChannel( void ) const { return _channels; };

/* ---------------------------------- Coplien's f. ---------------------------------- */
Server::Server(int port, std::string const &password) : _password(password) {

	_numClient = 0;
	/* create a socket (IPV4, TCP) */
	this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (-1 == this->_fd)
		throw Server::init_error((char *)"failed to create a socket");

	/* remove a binding error (bind even if the port is already use) */
	int	opt = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw Server::init_error((char *)"setsockopt(SO_REUSEADDR) failed");

	/* binding to the port */
	this->_sockAddr.sin_family = AF_INET;
	this->_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY); // use my IP address
	this->_sockAddr.sin_port = htons(port); // htons convert number to network byte order

	if (bind(this->_fd, (struct sockaddr *)&this->_sockAddr, sizeof(sockaddr)) < 0)
		throw Server::init_error((char *)"failed during binding");

	if (listen(this->_fd, MAX_WAIT) < 0)
		throw Server::init_error((char *)"failed to listen on socket");

	std::cout << "listen on port " << port << std::endl;
}

Server::~Server() {

	/* destroy the server */
	std::map<int, Client *>::iterator	it = _clients.begin();

	for (; it != _clients.end(); ++it)
		delete it->second;

	for (int i = 0; i < (int)_channels.size(); i++)
		delete _channels[i];

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
		FD_ZERO(&this->_writefds);
		FD_SET(this->_fd, &this->_readfds); // add server ID to socket_set
		max_sd = this->_fd;

		/* add all client ID to socket_set */
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {

			int	sd = it->second->getFd();
			if (sd > 0) {
				FD_SET(sd, &this->_readfds);
				if (it->second->getWritting())
					FD_SET(sd, &this->_writefds);
			}
			if (sd > max_sd)
				max_sd = sd;
		}

		struct timeval	tv = { 0, 50000 };
		activity = select(max_sd + 1, &this->_readfds, &this->_writefds, NULL, &tv);
		if (activity < 0 && errno != EINTR)
			throw Server::run_error((char *)"select failed");

		/* incoming connection */
		if (FD_ISSET(this->_fd, &this->_readfds))
			if (_numClient < MAX_CLIENT)
				this->addClient();

		/* I/O (input/output) operation from client */
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			int	sd = it->first;

			if (FD_ISSET(sd, &_readfds))
				if (!this->readFromClient(sd))
					break ;
		}

		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			int	sd = it->first;

			if (it->second->getWritting())
				if (FD_ISSET(sd, &_writefds))
					this->sendToClient(sd);
		}
	}
}

void	Server::sendToClient(int sd) {

	std::string	to_send = _clients[sd]->getMsg();

	send(sd, to_send.c_str(), to_send.size(), 0);
	_clients[sd]->setWritting(false);
	_clients[sd]->setMsg("");
}

bool	Server::removeClient(int sd) {

	delete _clients.find(sd)->second;
	_clients.erase(_clients.find(sd));
	_numClient--;
	return false;
}

bool	Server::readFromClient(int sd) {

	ssize_t	b_read = -1;
	char	buff[BUFFER_SIZE] = {0};
	std::string	user = _clients.find(sd)->second->getUsername();

	b_read = recv(sd, buff, BUFFER_SIZE, 0);

	/* destroy the client */
	if (b_read < 1)
		return this->removeClient(sd);
	else {
		std::string	msg = _mtos(buff);
		if (!_clients[sd]->treatRequest(msg, this))
			return this->removeClient(sd);
	}
	return true;
}

void	Server::addClient(void) {

	std::cout << "someone wants to connect to the server" << std::endl;

	/* accept the client */
	socklen_t	addr_len = sizeof(_sockAddr);
	int	new_socket = accept(_fd, (struct sockaddr *)&_sockAddr, &addr_len);

	if (new_socket < 0)
		throw Server::run_error((char *)"failed during acceptation");

	/* add the new socket to the socket_set */
	this->_clients[new_socket] = new Client(new_socket, this->giveNickname());
	_numClient++;
}

std::string	Server::giveNickname(void) {

	int	counter = -1;
	while (++counter < MAX_CLIENT) {
		if (!this->isAlreadyTaken(lst_name[counter]))
			return lst_name[counter];
	}
	return "user??";
}

bool	Server::isAlreadyTaken(std::string const &str) {

	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if (!str.compare(it->second->getNickname()))
			return true;
	return false;
}

Channel	*Server::addChannel(std::string const &name) {

	Channel	*new_channel = new Channel(name);
	_channels.push_back(new_channel);
	return new_channel;
}