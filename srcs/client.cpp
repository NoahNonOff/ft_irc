#include "client.hpp"

int const			&Client::getFd(void) const { return _fd; }
std::string const	&Client::getUsername(void) const { return _username; }
std::string const	&Client::getNickname(void) const { return _nickname; }

std::string const	Client::getMsg(void) {

	std::string	ret = "";

	if (_toSend.size()) {
		ret = _toSend[0];
		_toSend.erase(_toSend.begin());
	}
	return ret + "\r\n"; // add the CR-LF pair
}

bool	Client::hasMsg(void) const { return _toSend.size(); }

Client::Client(const int clt_fd, const t_serverInfo &inf) : _fd(clt_fd), _serverInfo(inf) {

	_nickname = "";
	_username = "";
	_registration = 0b00000000;
	_msg = "";
}

Client::~Client(void) {

	std::cout << _nickname << " (" << _fd << "): quit the server" << std::endl;
	close(_fd);
}

/////////////////////////////////////////////////////////////

void	Client::treatRequest(std::string const &request, Server *server, bool &quit) {

	Request			rqst(request);

	if (rqst.empty())
		return ;

	if (rqst.is("CAP"))
		capCMD(rqst);
	else if (rqst.is("USER"))
		userCMD(rqst);
	else if (rqst.is("PASS"))
		passCMD(rqst, server);
	else if (rqst.is("NICK"))
		nickCMD(rqst, server);
	else if (rqst.is("PING"))
		pingCMD(rqst);
	else if (rqst.is("QUIT"))
		quitCMD(rqst, quit);
	else if (!validated(_registration))
		err(ERR_NOTREGISTERED);
	else
		err(ERR_UNKNOWNCOMMAND, rqst.getCommand().c_str());

	if (!_msg.empty()) {
		_toSend.push_back(_msg);
		_msg = "";
	}
}

/////////////////////////////////////////////////////////////

void	Client::err(const int err, ...) {

	va_list				args;
	std::stringstream	ss;
	ss << std::setw(3) << std::setfill('0') << err;
	va_start(args, err);
	_msg = ":" + _serverInfo.name + " " + ss.str() + _nickname + " ";

	switch (err) {
		case ERR_NEEDMOREPARAMS :
			_msg += std::string(va_arg(args, char *)) + " :Not enough parameters";
			break ;
		case ERR_NONICKNAMEGIVEN :
			_msg += ":No nickname given";
			break ;
		case ERR_ERRONEUSNICKNAME :
			_msg += std::string(va_arg(args, char *)) + " :Erroneus nickname";
			break ;
		case ERR_NICKNAMEINUSE :
			_msg += std::string(va_arg(args, char *)) + " :Nickname is already in use";
			break ;
		case ERR_NOTREGISTERED :
			_msg += ":You have not registered";
			break ;
		case ERR_UNKNOWNCOMMAND :
			_msg += std::string(va_arg(args, char *)) + " :Unknown command";
			break ;
		case ERR_ALREADYREGISTERED :
			_msg += ":You may not reregister";
			break ;
		case ERR_PASSWDMISMATCH :
			_msg += ":Password incorrect";
			break ;
		default:
			_msg += "Unknown Error";
	}
	va_end(args);
}

// RPL_ISUPPORT
void	Client::rpl(const int rpl, ...) {

	va_list				args;
	std::stringstream	ss;
	ss << std::setw(3) << std::setfill('0') << rpl;
	va_start(args, rpl);
	_msg = ":" + _serverInfo.name + " " + ss.str() + _nickname + " ";

	switch (rpl) {
		case RPL_YOURHOST :
			_msg += ":Your host is " + _serverInfo.name + ", running version " + _serverInfo.version;
			break ;
		default:
			_msg += "Unknown Reply";
	}
	va_end(args);
}