#include "server.hpp"

int const						&Server::getFd( void ) const { return _fd; }
int const						&Server::getPort( void ) const { return _port; }
std::string const				&Server::getPassword(void) const { return _password; }
std::map<int, Client *> const	&Server::getClients( void ) const { return _clients; }

Server::Server(int port, std::string const &password, int h) : _pHash(h), _password(password) {

	(void)_pHash;
	/* set default value */
	_port = port;
	_numClient = 0;
	_maxWait = 10;
	_maxClts = 5;
	_maxChannel = 3;
}

Server::~Server(void) {

	/* destroy the server */
	std::map<int, Client *>::iterator	it = _clients.begin();

	for (; it != _clients.end(); ++it)
		delete it->second;

	_clients.clear();
	close(_fd);
	std::cout << "Server closed" << std::endl;
}

/////////////////////////////////////////////////////////////

void	Server::init(void) {

	/* create a socket (IPV4, TCP) */
	this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (-1 == this->_fd)
		throw Server::serverError((char *)"failed to create a socket");

	/* remove a binding error (bind even if the port is already use) */
	int	opt = 1;
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw Server::serverError((char *)"setsockopt(SO_REUSEADDR) failed");

	/* binding to the port */
	this->_sockAddr.sin_family = AF_INET;
	this->_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY); // use my IP address
	this->_sockAddr.sin_port = htons(_port); // htons convert number to network byte order

	if (bind(this->_fd, (struct sockaddr *)&this->_sockAddr, sizeof(sockaddr)) < 0)
		throw Server::serverError((char *)"failed during binding");

	if (listen(this->_fd, _maxWait) < 0)
		throw Server::serverError((char *)"failed to listen on socket");

	std::cout << "listen on port " << _port << std::endl;
}

void	Server::getConfig(const std::string &filename) {

	JSON::Object	*json = JSON::parseFile(filename);
	JSON::Atype		*server = json->get("server");

	if (!server->isNull()) {

		if (!server->get("maxWait")->isNull())
			_maxWait = server->get("maxWait")->toNum();
		if (!server->get("maxClient")->isNull())
			_maxClts = server->get("maxClient")->toNum();
		if (!server->get("maxChannels")->isNull())
			_maxChannel = server->get("maxChannels")->toNum();
	}

	delete json;
}

void	Server::run(void) {

	int		max_sd, activity;

	std::cout << "waiting for connections ..." << std::endl;
	while (1) {

		FD_ZERO(&this->_fds.readfds); // clear socket_set
		FD_ZERO(&this->_fds.writefds);
		FD_SET(this->_fd, &this->_fds.readfds); // add server ID to socket_set
		max_sd = this->_fd;

		/* add all client ID to socket_set */
		for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {

			int	sd = it->second->getFd();
			if (sd > 0) {
				FD_SET(sd, &this->_fds.readfds);
				if (it->second->hasMsg())
					FD_SET(sd, &this->_fds.writefds);
			}
			if (sd > max_sd)
				max_sd = sd;
		}

		activity = select(max_sd + 1, &this->_fds.readfds, &this->_fds.writefds, NULL, NULL);
		if (activity < 0 && errno != EINTR)
			throw Server::serverError((char *)"select failed");

		/* incoming connection */
		if (FD_ISSET(this->_fd, &this->_fds.readfds))
			if (this->_numClient < _maxClts)
				this->addClient();

		/* I/O (input/output) operation from client */
		for (clt_iterator it = _clients.begin(); it != _clients.end(); ++it) {

			int	sd = it->first;

			if (FD_ISSET(sd, &_fds.readfds))
				if (!this->readFromClient(sd))
					break ;

			if (it->second->hasMsg())
				if (FD_ISSET(sd, &this->_fds.writefds))
					this->sendToClient(sd);
		}
	}
}

/////////////////////////////////////////////////////////////

void	Server::sendToClient(int sd) {

	std::string	to_send = _clients[sd]->getMsg();

	send(sd, to_send.c_str(), to_send.size(), 0);
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
	char	buff[BUFFER_SIZE] = { 0 };

	b_read = recv(sd, buff, BUFFER_SIZE, 0);

	if (b_read < 1) {
		return this->removeClient(sd);		/* destroy the client */
	} else {
		std::string	msg = mtos(buff);
		std::cout << "<" << msg << ">" << std::endl;

		if (!_clients[sd]->treatRequest(msg, this))
			return this->removeClient(sd);
	}
	return true;
}

void	Server::addClient(void) {

	/* accept the client */
	socklen_t	addr_len = sizeof(_sockAddr);
	int	new_socket = accept(_fd, (struct sockaddr *)&_sockAddr, &addr_len);

	if (new_socket < 0)
		throw Server::serverError((char *)"failed during acceptation");

	/* add the new socket to the socket_set */
	std::cout << "a new user (" << new_socket << ") is now connected to the server" << std::endl;
	this->_clients[new_socket] = new Client(new_socket);
	_numClient++;
}

