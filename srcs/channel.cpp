// channel.cpp
//
// Author: Noah BEAUFILS
// Date: 9-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
std::string	const &Channel::getName(void) const { return _name; }
std::string	const &Channel::getPassword( void ) const { return _password; }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Channel::Channel(std::string const &name) : _name(name) {
	/* initiate the channel */
	_password = "";
}

Channel::~Channel() {
	/* destroy the channel */
}

/* ----------------------------------- functions ----------------------------------- */
