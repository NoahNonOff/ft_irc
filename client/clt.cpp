// clt.cpp
//
// Author: Noah BEAUFILS
// Date: 10-oct-2023

#include "clt.hpp"

Clt::Clt(std::string const &host, int port) : _host(host), _port(port) {

	/* initiate the client */
	if (-1 == (_fd = socket(AF_INET, SOCK_STREAM, 0)))
		throw std::runtime_error("socket");

	struct sockaddr_in	servaddr;
	bzero(&servaddr, sizeof(servaddr)); // clean the struct

	/* assign IP, PORT */
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(_host.c_str());
	servaddr.sin_port = htons(_port);

	/* connect the client socket to server socket */
	if (connect(_fd, (SA*)&servaddr, sizeof(servaddr)))
		throw std::runtime_error("connection failed");

	_prompt = "";
	_msg = "";
	_is_msg = false;

	system("clear"); // clear the console
	std::cout << "\t\e[36m\e[1m[ ==== IRC Client ==== ]\e[0m\n" << std::endl;
	std::cout << "Connecting with duck " << _host << " " << _port << "..." << std::endl;
}

Clt::~Clt() {
	/* destroy the client */
	close(_fd);
}

/* \0337 -> save current state */
/* \0338 -> restore the most recent state */
/* \033[J -> erase from cursor to end */
/* \r -> return to the beginning of the line */

void	Clt::clientLoop( void ) {

	std::cout << PROMPT;
	fflush(stdout);
	/* modified the console usage */
	tcgetattr(STDIN_FILENO, &_oldt);
	_newt = _oldt;
	_newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &_newt);

	while (true) {

		fd_set	fds;
		FD_ZERO(&fds);
		FD_SET(0, &fds); // check at input


		if (select(1, &fds, NULL, NULL, 0) < 0)
			break ;

		// if (_is_msg) {
		// 	this->treatRequest();
		// }

		/* we can read from the server */
		if (FD_ISSET(0, &fds))
			this->treatChar();

		fflush(stdout);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &_oldt);
	std::cout << "connection closed by foreigned host" << std::endl;
}

void	Clt::treatChar(void) {

	char	c, buff[2] = { 0 };
	if (read(0, buff, 1) < 1)
		return ;

	c = buff[0];
	tcsetattr(STDIN_FILENO, TCSANOW, &_oldt);
	if (c == '\n') {
		_prompt += c;
		this->removePrompt();
		std::cout << PROMPT;
		send(_fd, (void *)_prompt.c_str(), _prompt.size(), 0);
		_prompt = "";
	}
	else if (c == 127)
		this->do_backspace();
	else {
		_prompt += c;
		std::cout << c;
	}
	fflush(stdout);
	tcsetattr(STDIN_FILENO, TCSANOW, &_newt);
}

void	Clt::removePrompt(void) {

	for (int i = 0; i < int(PROMPT_SIZE + _prompt.size()); i++)
		std::cout << "\033[1D";
	std::cout << "\033[J";
}

void	Clt::do_backspace(void) {

	if (_prompt.size() <= 0)
		return ;
	_prompt = _prompt.substr(0, _prompt.size() - 1);
	std::cout << "\033[1D\033[J";
}

/*
bool	Clt::treatRequest(void) {

	char	buff[BUFFER_SIZE] = { 0 };

	if (recv(_fd, buff, BUFFER_SIZE, 0) < 1)
		return false;

	//std::cout << "\0338"; // restore last save
	std::cout << buff;
	// std::cout << "\0337"; // save the current state
	std::cout << PROMPT << _prompt;
	fflush(stdout);
	return true;
}
*/