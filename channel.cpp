// channel.cpp
//
// Author: Noah BEAUFILS
// Date: 5-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
std::string	const &Channel::getName(void) const { return _name; } 

/* ---------------------------------- Coplien's f. ---------------------------------- */
Channel::Channel(std::string const &name) : _name(name) {
	/* initiate the channel */
}

Channel::~Channel() {
	/* destroy the channel */
}

/* ----------------------------------- functions ----------------------------------- */
