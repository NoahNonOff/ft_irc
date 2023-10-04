// client.cpp
//
// Author: Noah BEAUFILS
// Date: 4-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
int const	&Client::getFd(void) const { return _fd; }
std::string const	&Client::getUsername( void ) const { return _username; }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Client::Client(int const clt_fd) : _fd(clt_fd) {

	_username = "unnamed_user";
	_validated = false;
	_validateTry = 3;

	_is_msg = false;
	_msg_to_send = "";
}

Client::~Client() {
	close(_fd);
}
