// channel.cpp
//
// Author: Noah BEAUFILS
// Date: 15-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
bool const	&Channel::getTopicRestriction(void) const { return _topicRestriction; }
bool const	&Channel::getInviteRestriction(void) const { return _inviteOnly; }
std::string	const &Channel::getName(void) const { return _name; }
std::string	const &Channel::getTopic(void) const { return _topic; }
std::string	const &Channel::getPassword(void) const { return _password; }
std::map<Client *, bool> const &Channel::getAdmin(void) const { return _admin; }

void	Channel::setTopic(std::string const &n) { _topic = n; }

/* ---------------------------------- Coplien's f. ---------------------------------- */
Channel::Channel(std::string const &name) : _name(name) {
	/* initiate the channel */
	_password = "";
	_topicRestriction = true;
	_topic = "no topic yet";
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

void	Channel::kickUser(Client *clt) {

	clt->setChannel(NULL);
	clt->addMsg("\x1B[31m\x1B[1mYou have been kick from the server\x1B[0m\n");
	removeUser(clt);
}

bool	Channel::addUser(Client *clt, bool admin) {

	if (_admin.find(clt) != _admin.end())
		return true;
	if (!_userLimit || _userLimit > (int)_admin.size()) {
		_admin[clt] = admin;
		return true;
	}
	clt->addMsg("\x1B[31merror: too many people in the server\x1B[0m\n");
	clt->setChannel(NULL);
	return false;
}

void	Channel::broadcast(std::string const &msg, Client *clt) {

	std::map<Client *, bool>::iterator	it = _admin.begin();
	if (msg.size() < 1) {
		for (;it != _admin.end(); it++) {
			if (it->first == clt || it->first->getChannel() != this)
				continue ;
			it->first->addMsg(clt->getUsername() + " quit the channel\n");
		}
	}
	else
		for (;it != _admin.end(); it++)
			if (it->first->getChannel() == this)
				it->first->addMsg("[public]" + clt->getUsername() + ": " + msg + "\n");
}

bool	Channel::isAdmin(Client *clt) {

	if (_admin.find(clt) == _admin.end())
		return false;
	return _admin.find(clt)->second;
}

Client	*Channel::isInChannel(std::string const &name) {

	for (std::map<Client *, bool>::iterator it = _admin.begin(); it != _admin.end(); it++)
		if (!it->first->getNickname().compare(name))
			return it->first;
	return NULL;
}

/* ----------------------------------- options ----------------------------------- */

void	Channel::mode_t(void) { _topicRestriction = _topicRestriction ? false : true; }

void	Channel::mode_i(Client *clt) {
	if (_inviteOnly)
		clt->addMsg("you remove invite only mode\n");
	else
		clt->addMsg("you set invite only mode\n");
	_inviteOnly = _inviteOnly ? false : true;
}

void	Channel::mode_k(std::vector<std::string> commands, Client *clt) {

	if (commands.size() == 2) {
		_password = "";
		clt->addMsg("the channel-key was removed\n");
		return ;
	}
	_password = commands[2];
	clt->addMsg("the channel-key is now " + commands[2] + "\n");
}

void	Channel::mode_o(std::vector<std::string> commands, Client *clt) {

	Client	*target = NULL;
	if (commands.size() != 3) {
		clt->addMsg("\x1B[31m/mode o <nickname>\x1B[0m\n");
		return ;
	}
	if ((target = this->isInChannel(commands[2])))
		_admin[target] = this->isAdmin(target) ? false : true; 
	else
		clt->addMsg("\x1B[31merror: " + commands[2] + ": user not found\x1B[0m\n");
}

void	Channel::mode_l(std::vector<std::string> commands, Client *clt) {

	int	num = 0;
	if (commands.size() == 2) {
		_userLimit = 0;
		clt->addMsg("the user limit was removed\n");
		return ;
	}
	try {
		num = _stoi(commands[2]);
	} catch (std::length_error &error) {
		clt->addMsg("\x1B[31merror: " + commands[2] + ": bad number\x1B[0m\n");
	}
	std::cout << _admin.size() << std::endl;
	if (num < (int)_admin.size())
		clt->addMsg("\x1B[31merror: too many person are already in the server\x1B[0m\n");
	else
		_userLimit = num;
}

