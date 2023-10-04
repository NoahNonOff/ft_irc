// client.cpp
//
// Author: Noah BEAUFILS
// Date: 3-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
int const	&Client::getFd(void) const { return _fd; }
std::string const &Client::getPassword(void) const { return _password; };

/* ---------------------------------- Coplien's f. ---------------------------------- */
Client::Client(int const clt_fd, std::string const &password) : _fd(clt_fd), _password(password) {
	/* initialize the client */
}

Client::~Client() {
	/* destroy the client */
	close(_fd);
}

/* ----------------------------------- functions ----------------------------------- */
