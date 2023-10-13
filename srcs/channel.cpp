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

void	Channel::broadcast(std::string const &msg, Client *clt) {

	std::map<Client *, bool>::iterator	it = _admin.begin();
	if (msg.size() < 1) {
		for (;it != _admin.end(); it++) {
			if (it->first == clt)
				continue ;
			it->first->addMsg(clt->getUsername() + " quit the channel\n");
		}
	}
	else
		for (;it != _admin.end(); it++)
			it->first->addMsg("[public]" + clt->getUsername() + ": " + msg + "\n");
}