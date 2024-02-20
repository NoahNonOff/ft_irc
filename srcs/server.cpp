#include "server.hpp"

const int						&Server::getFd( void ) const { return _fd; }
const int						&Server::getPort( void ) const { return _port; }
const std::map<int, Client *>	&Server::getClients( void ) const { return _clients; }

bool	Server::isPassword(const std::string &key) const {

	return (_pHash ? !hash::md5(key).compare(_password) : !hash::sha256(key).compare(_password));
}

Server::Server(int port, std::string const &password, int h) : _pHash(h), _password(password) {

	/* set default value */
	_port = port;
	_numClient = 0;
	_maxWait = 10;
	_maxClts = 5;
	_maxChannel = 3;
	_serverInfos.name = "IRC_default";
	_serverInfos.version = "~";
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
		if (!server->get("servername")->isNull())
			_serverInfos.name = server->get("servername")->toString();
		if (!server->get("version")->isNull())
			_serverInfos.version = server->get("version")->toString();
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

	std::cout << "send: \"" << mtos((char *)to_send.c_str()) << "\"" << std::endl;
	send(sd, to_send.c_str(), to_send.size(), 0);
}

bool	Server::removeClient(int sd) {

	delete _clients.find(sd)->second;
	_clients.erase(_clients.find(sd));
	_numClient--;
	return false;
}

bool	Server::readFromClient(int sd) {

	ssize_t	b_read = -1;
	bool	quit = false;
	char	buff[BUFFER_SIZE] = { 0 };

	b_read = recv(sd, buff, BUFFER_SIZE, 0);

	if (b_read < 1) {
		return this->removeClient(sd);		/* destroy the client */
	} else {

		/* if the message is not terminated by "\r\n", ignore it */
		if (b_read > 2 && !(buff[b_read - 1] == '\n' && buff[b_read - 2] == '\r'))
			return true;
		std::string	msg = mtos(buff);
		std::cout << "<" << msg << ">" << std::endl;

		_clients[sd]->treatRequest(msg, this, quit);
		if (quit)
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
	this->_clients[new_socket] = new Client(new_socket, _serverInfos);
	_numClient++;
}

bool	Server::validNick(const std::string &nick, bool &valid) const {

	valid = false;
	for (size_t i = 0; i < nick.size(); i++)
		if (!isalnum(nick[i]) && nick[i] != '{' && nick[i] != '}' && nick[i] != '[' &&
			nick[i] != ']' && nick[i] != '\\' && nick[i] != '|')
			return false;
	valid = true;
	for (const_clt_iterator it = _clients.begin(); it != _clients.end(); it++)
		if (!it->second->getNickname().compare(nick))
			return false;
	return true;
}
