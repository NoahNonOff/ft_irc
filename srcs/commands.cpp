#include "client.hpp"

void	Client::capCMD(void) {

	_msg = "CAP * LS :\r\n";
	addFlag(_registration, F_CAP);
}

void	Client::userCMD(Request rqst) {

	if (validated(_registration)) {
		err(ERR_ALREADYREGISTERED);	/* already registered */
		return ;
	}

	else if (rqst.size() < 5) {
		err(ERR_NEEDMOREPARAMS, rqst.getCommand().c_str());
		return ;
	}

	/* check the validity of the command */
	else if (!rqst[1].compare(_nickname) && !rqst[2].compare("0") && !rqst[3].compare("*")) {

		_username = rqst[4];
		if (_username.empty())
			err(ERR_NEEDMOREPARAMS, rqst.getCommand().c_str()); /* new username is empty */
		addFlag(_registration, F_USER);
	}
}

void	Client::nickCMD(Request rqst, Server *server) {

	if (rqst.size() < 2) {
		err(ERR_NONICKNAMEGIVEN);
		return ;
	}

	bool	valid = true;
	if (!server->validNick(rqst[1], valid)) {
		if (!valid)
			err(ERR_ERRONEUSNICKNAME, rqst[1].c_str());	/* nick isn't valid */
		else
			err(ERR_NICKNAMEINUSE, rqst[1].c_str());	/* nick already use */
		return ;
	}
	_nickname = rqst[1];
	_msg = "NICK " + rqst[1] + "\r\n";
	addFlag(_registration, F_NICK);
}

void	Client::passCMD(Request rqst, Server *server) {

	if (validated(_registration)) {
		err(ERR_ALREADYREGISTERED);
		return ;
	}

	else if (rqst.size() < 2) {
		err(ERR_NEEDMOREPARAMS, rqst.getCommand().c_str());
		return ;
	}

	else if (!server->isPassword(rqst[0])) {
		err(ERR_PASSWDMISMATCH);
		return ;
	}
	addFlag(_registration, F_PASS);
}

void	Client::pingCMD(Request rqst) {

	if (rqst.size() < 2)
		err(ERR_NEEDMOREPARAMS, rqst.getCommand().c_str());
	else
		_msg = "PONG " + rqst[1] + "\r\n";
}

void	Client::quitCMD(Request rqst, bool &quit) {

	(void)rqst;
	quit = false;
}