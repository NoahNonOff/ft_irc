#include "irc.hpp"

int const			&Client::getFd(void) const { return _fd; }
std::string const	&Client::getUsername(void) const { return _username; }
std::string const	&Client::getNickname(void) const { return _nickname; }
std::string const	&Client::getMsg(void) const { return _msg; }

bool	Client::hasMsg(void) { return !(_msg.empty()); }
void	Client::setMsg(const std::string &str) { _msg = str; }

Client::Client(const int clt_fd) : _fd(clt_fd) {

	_nickname = "";
	_username = "";
	_validated = false;
	_msg = "";
}

Client::~Client(void) {

	std::cout << _nickname << " (" << _fd << "): quit the server" << std::endl;
	close(_fd);
}

/////////////////////////////////////////////////////////////

bool	Client::treatRequest(std::string const &request, Server *server) {
	
	return this->executeCommand(request, server);
}

bool	Client::executeCommand(std::string const &command, Server *server) {

	arg_array	cmds = splitCmds(command);

	(void)server;
	if (cmds.empty())
		return true;

	if (!cmds[0].compare("CAP"))
		this->capCMD(cmds);
	else if (!cmds[0].compare("PING"))
		this->pingCMD(cmds);
	else if (!cmds[0].compare("QUIT"))
		return false;
	return true;
}

/////////////////////////////////////////////////////////////

void	Client::capCMD(arg_array cmds) {

	(void)cmds;
	std::cout << "cap cmd" << std::endl;
	_msg += "001 user\r\n";
}

void	Client::pingCMD(arg_array cmds) {

	(void)cmds;
	std::cout << "ping cmd" << std::endl;
	_msg += "pong\r\n";
}
