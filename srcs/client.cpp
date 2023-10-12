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

void	Client::setMsg(std::string const &n) { this->setWritting(true); _msg_to_send = n; }
void	Client::setWritting(bool n) { _is_msg = n; }
void	Client::setValidation(bool n) { _validated = n; }

void	Client::addMsg(std::string const &n) { _msg_to_send.append(n); }
void	Client::setMp(std::string const &msg, std::string const &src) {

	if (!this->getValidation())
		return ;
	if (!this->getWritting()) {
		this->setWritting(true);
		this->setMsg("[private]" + src + ": " + msg + "\n");
	}
	else
		this->addMsg("[private]" + src + ": " + msg + "\n");
}

/* ---------------------------------- Coplien's f. ---------------------------------- */
Client::Client(int const clt_fd, std::string const &nickname) : _fd(clt_fd), _nickname(nickname) {

	_channel = NULL;
	_username = "lil'one";
	_validated = false;
	_validateTry = 3;

	_is_msg = false;
	_msg_to_send = "";

	/* send the first prompt */
	std::string msg = "please enter the password:\n";
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

	return ret;
}

bool	Client::secure_connection(std::string const &request, std::string const &password) {


	this->setWritting(true);
	if (!password.compare(request)) {
		this->setValidation(true);
		this->setMsg("good password, you are now connected\n");
		return true;
	}
	this->setMsg("bad password, please try again:\n");
	if (--_validateTry < 1)
		return false;
	return true;
}

bool	Client::executeCommand(std::string const &command, Server *server) {

	std::vector<std::string>	commands = splitCmds(command);

	this->setWritting(true);
	if (commands.size() < 1)
		return true;
	if (!commands[0].compare("user"))
		this->userCMD();
	else if (!commands[0].compare("quit"))
		return false;
	else if (!commands[0].compare("help"))
		this->helpCMD();
	else if (!commands[0].compare("name"))
		this->nameCMD(commands);
	else if (!commands[0].compare("nick"))
		this->nickCMD(commands, server);
	else if (!commands[0].compare("chat"))
		this->chatCMD(commands, server);
	return true;
}

void	Client::launchMessage(std::string const &request) {

	std::string	msg = request;
	if (!this->_channel) {
		if (msg.size() > 0) {
			this->setMsg(msg + "\n");
		}
		return ;
	}
	// channel->broadcast(msg);
}

/* ------------------------------------ commands ----------------------------------- */
void	Client::userCMD(void) {

	std::string	info = "user: " + this->getNickname() \
	+ " " + this->getUsername() + "\n";

	this->addMsg(info);
}

void	Client::helpCMD(void) {

	std::string		line;
	std::string		help = "\x1B[34m";
	std::ifstream	f_in("help.file");

	while (std::getline(f_in, line)) {
		help.append(line + "\n");
	}
	this->addMsg(help + "\x1B[0m");
}

void	Client::nameCMD(std::vector<std::string> commands) {
	if (commands.size() < 2)
		this->addMsg("\x1B[31m/name <new_name>\x1B[0m\n");
	else
		this->_username = commands[1];
}

void	Client::nickCMD(std::vector<std::string> commands, Server *server) {
	if (commands.size() < 2) {
		this->addMsg("\x1B[31m/nick <new_nickname>\x1B[0m\n");
		return ;
	}
	if (!server->isAlreadyTaken(commands[1]))
		this->_nickname = commands[1];
	else
		this->addMsg("\x1B[31m" + commands[1] + ": error: this nickname is already used by someone else\n\x1B[0m");
}

void	Client::chatCMD(std::vector<std::string> commands, Server *server) {

	if (commands.size() < 3 || !commands[1].compare(_nickname)) {
		this->addMsg("\x1B[31m/chat <nickname_of_interlocutor> <message>\x1B[0m\n");
		return ;
	}
	std::map<int, Client *>::const_iterator	it = server->getClients().begin();
	for (; it != server->getClients().end(); ++it) {
		std::cout << it->second->getNickname() << " " << commands[1] << " " << it->second->getNickname().compare(commands[1]) << std::endl;
		if (!it->second->getNickname().compare(commands[1])) {

			/* set the message */
			std::string	msg = "";
			for (int i = 2; i < (int)commands.size(); i++) {
				if (i != 2 && i < (int)commands.size())
					msg += " ";
				msg += commands[i];
			}
			msg += ".";
			it->second->setMp(msg, _nickname);
			return ;
		}
	}
	this->addMsg("\x1B[31m" + commands[1] + ": error: no nickname match" + "\x1B[0m\n");
}
