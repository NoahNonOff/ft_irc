#include "irc.hpp"

int const			&Client::getFd(void) const { return _fd; }
std::string const	&Client::getUsername(void) const { return _username; }
std::string const	&Client::getNickname(void) const { return _nickname; }
std::string const	&Client::getMsg(void) const { return _msg; }

bool	Client::hasMsg(void) { return !(_msg.empty()); }
void	Client::setMsg(const std::string &str) { _msg = str; }

Client::Client(const int clt_fd) : _fd(clt_fd) {

	_nickname = "";
	_username = "";
	_validated = false;
	_msg = "";
}

Client::~Client(void) {

	std::cout << _nickname << " (" << _fd << "): quit the server" << std::endl;
	close(_fd);
}

/////////////////////////////////////////////////////////////

void	Client::treatRequest(std::string const &request, Server *server, bool &quit) {

	Request	rqst(request);

	if (rqst.empty())
		return ;

	(void)server;
	if (rqst.is("CAP"))
		capCMD();
	else if (rqst.is("NICK"))
		nickCMD(rqst);
	else if (rqst.is("USER"))
		userCMD(rqst);
	else if (rqst.is("PASS"))
		passCMD(rqst);
	else if (rqst.is("PING"))
		pingCMD(rqst);
	else if (rqst.is("QUIT"))
		quitCMD(rqst, quit);

	if (!_validated) {
		// send error -> ERR_NOTREGISTERED (451)
		return ;
	}

	// if (...) {
	// 	<command>
	// } else
	// 	send error -> ERR_UNKNOWNCOMMAND (421)
}

/////////////////////////////////////////////////////////////

void	Client::capCMD(void) { _msg += "CAP * LS :\r\n"; }

void	Client::nickCMD(Request rqst) {

	(void)rqst;
}

void	Client::userCMD(Request rqst) {

	(void)rqst;
}

void	Client::passCMD(Request rqst) {

	(void)rqst;
}

void	Client::pingCMD(Request rqst) {

	// if (rqst.size() < 2)
	// 	//  send error -> ERR_NEEDMOREPARAMS (461)
	// else
		_msg += "PONG " + rqst[1] + "\r\n";
}

void	Client::quitCMD(Request rqst, bool &quit) {

	(void)rqst;
	quit = false;
}