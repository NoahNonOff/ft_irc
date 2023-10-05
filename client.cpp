// client.cpp
//
// Author: Noah BEAUFILS
// Date: 5-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
int const	&Client::getFd(void) const { return _fd; }
bool const	&Client::getValidation( void ) const { return _validated; }
bool const	&Client::getWritting( void ) const { return _is_msg; }
std::string const	&Client::getMsg( void ) const { return _msg_to_send; }
std::string const	&Client::getUsername( void ) const { return _username; }
std::string const	&Client::getPassword( void ) const { return _password; }

void	Client::setMsg( std::string const &n ) { _msg_to_send = n; }
void	Client::setWritting( bool n ) { _is_msg = n; }
void	Client::setValidation( bool n ) { _validated = n; }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Client::Client(int const clt_fd, std::string const &password) : _fd(clt_fd), _password(password) {

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
	close(_fd);
}

/* ----------------------------------- functions ----------------------------------- */
bool	Client::treatRequest(std::string const &request) {
	
	if (!_validated)
		return this->secure_connection(request);
	this->setWritting(true);
	this->setMsg(PROMPT);
	return true;
}

bool	Client::secure_connection(std::string const &request) {

	this->setWritting(true);

	if (!this->getPassword().compare(request)) {

		this->setValidation(true);
		this->setMsg(PROMPT);
		return true;
	}
	this->setMsg("\x1B[1m$> bad password, please try again: \x1B[0m");
	if (--_validateTry < 1)
		return false;
	return true;
}
