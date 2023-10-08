// channel.cpp
//
// Author: Noah BEAUFILS
// Date: 5-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
std::string	const &Channel::getName(void) const { return _name; }
std::string	const &Channel::getPassword( void ) const { return _password; }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Channel::Channel(std::string const &name, std::string const &password) : _name(name), _password(password) {
	/* initiate the channel */
}

Channel::~Channel() {
	/* destroy the channel */
}

/* ----------------------------------- functions ----------------------------------- */
