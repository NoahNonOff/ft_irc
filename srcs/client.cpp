// client.cpp
//
// Author: Noah BEAUFILS
// Date: 9-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
int const	&Client::getFd(void) const { return _fd; }
bool const	&Client::getValidation(void) const { return _validated; }
bool const	&Client::getWritting(void) const { return _is_msg; }
std::string const	&Client::getMsg(void) const { return _msg_to_send; }
std::string const	&Client::getUsername(void) const { return _username; }
std::string const	&Client::getNickname(void) const { return _nickname; }

void	Client::setMsg(std::string const &n) { _msg_to_send = n; }
void	Client::setWritting(bool n) { _is_msg = n; }
void	Client::setValidation(bool n) { _validated = n; }

void	Client::addMsg(std::string const &n) { _msg_to_send.append(n); }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Client::Client(int const clt_fd, std::string const &nickname) : _fd(clt_fd), _nickname(nickname) {

	_channel = NULL;
	_username = "lil'one";
	_validated = false;
	_validateTry = 3;

	_is_msg = false;
	_msg_to_send = "";

	/* send the first prompt */
	std::string msg = "\x1B[1m$> please enter the password: \x1B[0m";
	send(clt_fd, msg.c_str(), msg.size(), 0);
}

Client::~Client() {
	std::cout << _username << " (" << _fd << "): quit the server" << std::endl;
	close(_fd);
}

/* ----------------------------------- functions ----------------------------------- */
bool	Client::treatRequest(std::string const &request, Server *server) {
	
	bool ret = true;
	if (!_validated)
		return this->secure_connection(request, server->getPassword());

	if (is_request(request))
		ret = this->executeCommand(request, server);
	else
		this->launchMessage(request);

	this->setWritting(true);
	this->addMsg(PROMPT);
	return ret;
}

bool	Client::secure_connection(std::string const &request, std::string const &password) {

	this->setWritting(true);

	if (!password.compare(request)) {

		this->setValidation(true);
		this->setMsg(PROMPT);
		return true;
	}
	this->setMsg("\x1B[1m$> bad password, please try again: \x1B[0m");
	if (--_validateTry < 1)
		return false;
	return true;
}

bool	Client::executeCommand(std::string const &command, Server *server) {

	std::vector<std::string>	commands = splitCmds(command);

	(void)server;
	if (commands.size() < 1)
		return true;
	if (!commands[0].compare("user"))
		this->userCMD();
	else if (!commands[0].compare("quit"))
		return false;
	else if (!commands[0].compare("help"))
		this->helpCMD();
	// else if (!commands[0].compare("name"))
	// 	this->nameCMD(commands);
	// else if (!commands[0].compare("nick"))
	// 	this->nickCMD(commands, server);
	return true;
}

void	Client::launchMessage(std::string const &request) {

	std::string	msg = request;
	if (!this->_channel) {
		this->setMsg(msg + "\n");
		return ;
	}
	// channel->broadcast(msg);
}

/* ------------------------------------ commands ----------------------------------- */
void	Client::userCMD(void) {

	std::string	info = "\x1B[34muser: " + this->getNickname() \
	+ " " + this->getUsername() + "\x1B[0m\n";

	this->setMsg(info);
}

void	Client::helpCMD(void) {

	std::string		line;
	std::string		help = "\x1B[34m";
	std::ifstream	f_in("help.file");

	while (std::getline(f_in, line)) {
		help.append(line + "\n");
	}
	this->setMsg(help + "\x1B[0m");
}