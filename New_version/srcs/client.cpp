// client.cpp
//
// Author: Noah BEAUFILS
// Date: 16-jan-2024

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
int const	&Client::getFd(void) const { return _fd; }
bool const	&Client::getValidation(void) const { return _validated; }
bool const	&Client::getWritting(void) const { return _is_msg; }
// Channel *Client::getChannel(void) const { return _channel; }
std::string const	&Client::getMsg(void) const { return _msg_to_send; }
std::string const	&Client::getUsername(void) const { return _username; }
std::string const	&Client::getNickname(void) const { return _nickname; }

void	Client::setMsg(std::string const &n) { _msg_to_send = n; }
void	Client::setWritting(bool n) { _is_msg = n; }
// void	Client::setChannel(Channel *n) { _channel = n; }
void	Client::setValidation(bool n) { _validated = n; }

void	Client::addMsg(std::string const &n) { _is_msg = true; _msg_to_send.append(n); }

void	Client::setMp(std::string const &msg, std::string const &src) {
	this->addMsg("[private]" + src + ": " + msg + "\n");
}

/* ---------------------------------- Coplien's f. ---------------------------------- */
Client::Client(int const clt_fd) : _fd(clt_fd) {

	// _channel = NULL;
	_nickname = "lil'one";
	_username = "lil'one";
	_validated = false;
	_validateTry = 3;
	_channelAccess = true;

	_is_msg = false;
	_msg_to_send = "";
}

Client::~Client() {
	std::cout << _username << " (" << _fd << "): quit the server" << std::endl;
	// if (_channel) {
	// 	_channel->broadcast("", this);
	// 	_channel->removeUser(this);
	// }
	close(_fd);
}

/* ----------------------------------- functions ----------------------------------- */

bool	Client::treatRequest(std::string const &request, Server *server) {
	
	bool ret = true;
	// if (!_validated)
	// 	return this->secure_connection(request, server->getPassword());

	// if (_channel && !_channelAccess)
	// 	this->accessToChannel(request);
	// else if (is_request(request))
		ret = this->executeCommand(request, server);
	// else
	// 	this->launchMessage(request);

	return ret;
}

bool	Client::executeCommand(std::string const &command, Server *server) {

	std::vector<std::string>	commands = splitCmds(command);

	(void)server;
	if (commands.size() < 1)
		return true;

	if (!commands[0].compare("CAP"))
		this->capCMD(commands);
	else if (!commands[0].compare("PING"))
		this->pingCMD(commands);
	else if (!commands[0].compare("QUIT"))
		return false;
	// else if (!commands[0].compare("USER"))
	// 	this->userCMD(commands);
	// else if (!commands[0].compare("users"))
	// 	this->usersCMD();
	// else if (!commands[0].compare("part"))
	// 	this->partCMD();

	// else if (!commands[0].compare("help"))
	// 	this->helpCMD();
	// else if (!commands[0].compare("name"))
	// 	this->nameCMD(commands);
	// else if (!commands[0].compare("kick"))
	// 	this->kickCMD(commands);
	// else if (!commands[0].compare("topic"))
	// 	this->topicCMD(commands);
	// else if (!commands[0].compare("list"))
	// 	this->listCMD(server);
	// else if (!commands[0].compare("mode"))
	// 	this->modeCMD(commands);

	// else if (!commands[0].compare("nick"))
	// 	this->nickCMD(commands, server);
	// else if (!commands[0].compare("invite"))
	// 	this->inviteCMD(commands, server);
	// else if (!commands[0].compare("chat"))
	// 	this->chatCMD(commands, server);
	// else if (!commands[0].compare("join"))
	// 	this->joinCMD(commands, server);

	return true;
}

/* ----------------------------------- commands ------------------------------------ */

void	Client::capCMD(std::vector<std::string> commands) {

	(void)commands;
	std::cout << "cap cmd" << std::endl;
	this->addMsg("001 user\r\n");
}

void	Client::pingCMD(std::vector<std::string> commands) {

	(void)commands;
	std::cout << "ping cmd" << std::endl;
	this->addMsg("pong\r\n");
}
