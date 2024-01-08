// client.cpp
//
// Author: Noah BEAUFILS
// Date: 8-jan-2024

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
Client::Client(int const clt_fd, std::string const &nickname) : _fd(clt_fd), _nickname(nickname) {

	// _channel = NULL;
	_username = "lil'one";
	_validated = false;
	_validateTry = 3;
	_channelAccess = true;

	_is_msg = false;
	_msg_to_send = "";

	/* send the first prompt */
	std::string msg = "\x1B[35m\x1B[1mplease enter the password:\x1B[0m\n";
	send(clt_fd, msg.c_str(), msg.size(), 0);
}

Client::~Client() {
	std::cout << _username << " (" << _fd << "): quit the server" << std::endl;
	if (_channel) {
		// _channel->broadcast("", this);
		// _channel->removeUser(this);
	}
	close(_fd);
}

/* ----------------------------------- functions ----------------------------------- */
