// client.cpp
//
// Author: Noah BEAUFILS
// Date: 4-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
int const	&Client::getFd(void) const { return _fd; }
bool const	&Client::getValidation( void ) const { return _validated; }
bool const	&Client::getWritting( void ) const { return _is_msg; }
std::string const	&Client::getMsg( void ) const { return _msg_to_send; }
std::string const	&Client::getUsername( void ) const { return _username; }

void	Client::setMsg( std::string const &n ) { _msg_to_send = n; }
void	Client::setWritting( bool n ) { _is_msg = n; }
void	Client::setValidation( bool n ) { _validated = n; }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Client::Client(int const clt_fd) : _fd(clt_fd) {

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
