// channel.cpp
//
// Author: Noah BEAUFILS
// Date: 13-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
std::string	const &Channel::getName(void) const { return _name; }
std::string	const &Channel::getPassword( void ) const { return _password; }
std::map<Client *, bool> const &Channel::getAdmin( void ) const { return _admin; }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Channel::Channel(std::string const &name) : _name(name) {
	/* initiate the channel */
	_password = "";
	_topic = "";
	_userLimit = 0;
	_inviteOnly = false;
}

Channel::~Channel() {
	/* destroy the channel */
}

/* ----------------------------------- functions ----------------------------------- */

void	Channel::removeUser(Client *clt) {

	if (clt) {
		if (_admin.find(clt) != _admin.end())
			_admin.erase(_admin.find(clt));
	}
}

void	Channel::addUser(Client *clt, bool admin) {

	if (_admin.find(clt) != _admin.end())
		return ;
	_admin[clt] = admin;
}