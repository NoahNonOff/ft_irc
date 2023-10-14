// client.cpp
//
// Author: Noah BEAUFILS
// Date: 14-oct-2023

#include "irc.hpp"

/* ---------------------------------- Set and Get ---------------------------------- */
int const	&Client::getFd(void) const { return _fd; }
bool const	&Client::getValidation(void) const { return _validated; }
bool const	&Client::getWritting(void) const { return _is_msg; }
std::string const	&Client::getMsg(void) const { return _msg_to_send; }
std::string const	&Client::getUsername(void) const { return _username; }
std::string const	&Client::getNickname(void) const { return _nickname; }

void	Client::setMsg(std::string const &n) { _msg_to_send = n; }
void	Client::setWritting(bool n) { _is_msg = n; }
void	Client::setChannel(Channel *n) { _channel = n; }
void	Client::setValidation(bool n) { _validated = n; }

void	Client::addMsg(std::string const &n) { _is_msg = true; _msg_to_send.append(n); }

void	Client::setMp(std::string const &msg, std::string const &src) {
	this->addMsg("[private]" + src + ": " + msg + "\n");
}

/* ---------------------------------- Coplien's f. ---------------------------------- */
Client::Client(int const clt_fd, std::string const &nickname) : _fd(clt_fd), _nickname(nickname) {

	_channel = NULL;
	_username = "lil'one";
	_validated = false;
	_validateTry = 3;
	_channelAccess = true;

	_is_msg = false;
	_msg_to_send = "";

	/* send the first prompt */
	std::string msg = "\x1B[35m\x1B[1mplease enter the password:\x1B[0m\n";
	send(clt_fd, msg.c_str(), msg.size(), 0);
}

Client::~Client() {
	std::cout << _username << " (" << _fd << "): quit the server" << std::endl;
	if (_channel) {
		_channel->broadcast("", this);
		_channel->removeUser(this);
	}
	close(_fd);
}

/* ----------------------------------- functions ----------------------------------- */
bool	Client::treatRequest(std::string const &request, Server *server) {
	
	bool ret = true;
	if (!_validated)
		return this->secure_connection(request, server->getPassword());

	if (_channel && !_channelAccess)
		this->accessToChannel(request);
	else if (is_request(request))
		ret = this->executeCommand(request, server);
	else
		this->launchMessage(request);

	return ret;
}

void	Client::accessToChannel(std::string const &request) {

	_channelAccess = true;
	if (_channel->getPassword().compare(request)) {
		_channel = NULL;
		this->addMsg("\x1B[35mbad password, connection with the channel failed\x1B[0m\n");
	}
	else {
		_channel->addUser(this, false);
		this->addMsg("\x1B[35mgood password, you are now connected to the channel\x1B[0m\n");
	}
}

bool	Client::secure_connection(std::string const &request, std::string const &password) {

	if (!password.compare(request)) {
		this->setValidation(true);
		this->addMsg("\x1B[35m\x1B[1mgood password, you are now connected\x1B[0m\n");
		return true;
	}
	this->addMsg("\x1B[35m\x1B[1mbad password, please try again:\x1B[0m\n");
	if (--_validateTry < 1)
		return false;
	return true;
}

bool	Client::executeCommand(std::string const &command, Server *server) {

	std::vector<std::string>	commands = splitCmds(command);

	if (commands.size() < 1)
		return true;
	if (!commands[0].compare("user"))
		this->userCMD();
	else if (!commands[0].compare("users"))
		this->usersCMD();
	else if (!commands[0].compare("ping"))
		this->pingCMD();
	else if (!commands[0].compare("part"))
		this->partCMD();
	else if (!commands[0].compare("quit"))
		return false;

	else if (!commands[0].compare("help"))
		this->helpCMD();
	else if (!commands[0].compare("name"))
		this->nameCMD(commands);
	else if (!commands[0].compare("kick"))
		this->kickCMD(commands);
	else if (!commands[0].compare("topic"))
		this->topicCMD(commands);
	else if (!commands[0].compare("list"))
		this->listCMD(server);

	else if (!commands[0].compare("nick"))
		this->nickCMD(commands, server);
	else if (!commands[0].compare("chat"))
		this->chatCMD(commands, server);
	else if (!commands[0].compare("join"))
		this->joinCMD(commands, server);
	else if (!commands[0].compare("mode"))
		this->modeCMD(commands, server);

	else
		this->addMsg("\x1B[31merror: " + commands[0] + ": command not found\x1B[0m\n");
	return true;
}

void	Client::quitChannel(void) { _channel = NULL; }

void	Client::launchMessage(std::string const &request) {

	std::string	msg = request;
	if (msg.size() < 1)
		return ; 
	if (!this->_channel)
		this->addMsg(msg + "\n");
	else
		_channel->broadcast(msg, this);
}


/* ==================================== commands ==================================== */

void	Client::pingCMD(void) { this->addMsg("pong\n"); }

void	Client::partCMD(void) { 
	if (_channel)
		_channel->broadcast("", this);
	this->quitChannel();
}

void	Client::userCMD(void) {

	std::string	info = "user: " + this->getNickname() \
	+ "::" + this->getUsername() + "::" + (_channel ? _channel->getName() : "no channel") + "\n";

	this->addMsg(info);
}

void	Client::usersCMD(void) {

	if (!_channel) {
		this->userCMD();
		return ;
	}

	std::string							info;
	std::map<Client *, bool>			lst_client = _channel->getAdmin();
	std::map<Client *, bool>::iterator	it = lst_client.begin();

	for (; it != lst_client.end(); ++it) {
		if (it->first == this) {
			info = "\x1B[35muser: " + it->first->getNickname() \
			+ "::" + it->first->getUsername() + "::" + (it->second ? "admin" : "regular") + "\x1B[0m\n";
		}
		else {
			info = "user: " + it->first->getNickname() \
			+ "::" + it->first->getUsername() + "::" + (it->second ? "admin" : "regular") + "\n";
		}
		this->addMsg(info);
	}
}

void	Client::helpCMD(void) {

	std::string		line;
	std::string		help = "\x1B[34m";
	std::ifstream	f_in("help.file");

	while (std::getline(f_in, line)) {
		help.append(line + "\n");
	}
	this->addMsg(help + "\x1B[0m");
}

void	Client::nameCMD(std::vector<std::string> commands) {
	if (commands.size() < 2)
		this->addMsg("\x1B[31merror: /name <new_name>\x1B[0m\n");
	else
		this->_username = commands[1];
}

void	Client::nickCMD(std::vector<std::string> commands, Server *server) {
	if (commands.size() < 2) {
		this->addMsg("\x1B[31merror: /nick <new_nickname>\x1B[0m\n");
		return ;
	}
	if (!server->isAlreadyTaken(commands[1]))
		this->_nickname = commands[1];
	else
		this->addMsg("\x1B[31m" + commands[1] + ": error: this nickname is already used by someone else\n\x1B[0m");
}

void	Client::chatCMD(std::vector<std::string> commands, Server *server) {

	if (commands.size() < 3 || !commands[1].compare(_nickname)) {
		this->addMsg("\x1B[31merror: /chat <nickname_of_interlocutor> <message>\x1B[0m\n");
		return ;
	}
	std::map<int, Client *>::const_iterator	it = server->getClients().begin();
	for (; it != server->getClients().end(); ++it) {
		if (!it->second->getNickname().compare(commands[1])) {

			/* set the message */
			std::string	msg = "";
			for (int i = 2; i < (int)commands.size(); i++) {
				if (i != 2 && i < (int)commands.size())
					msg += " ";
				msg += commands[i];
			}
			it->second->setMp(msg, _nickname);
			return ;
		}
	}
	this->addMsg("\x1B[31m" + commands[1] + ": error: no nickname match" + "\x1B[0m\n");
}

void	Client::listCMD(Server *server) {

	std::vector<Channel *>	lst_channel = server->getChannel();
	if (lst_channel.size() < 1) {
		this->addMsg("\x1B[31merror: no channel yet\x1B[0m\n");
		return ;
	}
	this->addMsg("list:\n");
	for (int i = 0; i < (int)lst_channel.size(); i++)
		this->addMsg("\t" + lst_channel[i]->getName() + "\n");
	this->addMsg("\x1B[0m");
}

void	Client::joinCMD(std::vector<std::string> commands, Server *server) {

	if (commands.size() != 2) {
		this->addMsg("\x1B[31m/join <server_name>\x1B[0m\n");
		return ;
	}

	/* if you try to connect to the server */
	if (_channel && _channel->getName().compare(commands[1])) {
		this->addMsg("You are already in the channel\n");
		return ;
	}

	Channel	*old_channel = NULL;
	std::vector<Channel *>	lst_channel = server->getChannel();
	for (int i = 0; i < (int)lst_channel.size(); i++) {
		if (!lst_channel[i]->getName().compare(commands[1]))
			old_channel = lst_channel[i];
	}
	if (!old_channel) {
		/* create the channel with admin right */
		_channel = server->addChannel(commands[1]);
		_channel->addUser(this, true);
		this->addMsg("you have created " + commands[1] + "\n");
		return ;
	}
	/* join a channel with regular right */
	this->quitChannel();
	_channel = old_channel;
	if (old_channel->getPassword().size() > 0) {
		_channelAccess = false;
		this->addMsg("\x1B[35m[" + old_channel->getName() + "]: please enter the password:\x1B[0m\n");
	}
	else if (old_channel->addUser(this, false))
		this->addMsg("you have joinned " + commands[1] + "\n");
}

void	Client::kickCMD(std::vector<std::string> commands) {

	/* treat the errors (not in channel, arguments error, not the admin) */
	if (!_channel || commands.size() != 2 || !_channel->isAdmin(this)) {

		if (commands.size() != 2)
			this->addMsg("\x1B[31m/kick <nickname>\x1B[0m\n");
		else
			this->addMsg("\x1B[31merror: you have to be operator in a channel\x1B[0m\n");
		return ;
	}
	if (!_channel->isInChannel(commands[1]))
		this->addMsg("\x1B[31merror: " + commands[1] + ": user not found\x1B[0m\n");
	else
		_channel->kickUser(_channel->isInChannel(commands[1]));
}

void	Client::topicCMD(std::vector<std::string> commands) {

	/* treat the errors (not in channel, not the admin and restriction) */
	if (!_channel || (!_channel->isAdmin(this) && _channel->getTopicRestriction())) {
		this->addMsg("\x1B[31merror: you have to be operator in a channel\x1B[0m\n");
		return ;
	}
	if (commands.size() == 1)
		this->addMsg("topic: " + _channel->getTopic() + "\n");
	else {
		std::string	newTopic = "";
		for (int i = 1; i < (int)commands.size(); i++) {
			if (i != 1 && i < (int)commands.size())
				newTopic += " ";
			newTopic += commands[i];
		}
		newTopic += ".";
		_channel->setTopic(newTopic);
	}
}

void	Client::modeCMD(std::vector<std::string> commands, Server *server) {

	(void)server;
	if (commands.size() < 2) {

		this->addMsg("\x1B[31m/mode <options>\x1B[0m\n");
		return ;
	}
	if (!_channel || !_channel->isAdmin(this)) {
		this->addMsg("\x1B[31merror: you have to be operator in a channel\x1B[0m\n");
		return ;
	}
	if (!commands[1].compare("o"))
		_channel->mode_o(commands, this);
	else if (!commands[1].compare("l"))
		_channel->mode_l(commands, this);
	else if (!commands[1].compare("k"))
		_channel->mode_k(commands, this);
	else if (!commands[1].compare("t"))
		_channel->mode_t();
	else
		this->addMsg("\x1B[31merror: " + commands[1] + ": option not found\x1B[0m\n");
}