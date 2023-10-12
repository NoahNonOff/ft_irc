// clt.cpp
//
// Author: Noah BEAUFILS
// Date: 10-oct-2023

#include "clt.hpp"

struct timeval	tv = { 0, 5000 };

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

	system("clear"); // clear the console
	std::cout << "\t\e[36m\e[1m[ ==== IRC Client ==== ]\e[0m\n" << std::endl;
	std::cout << "Connecting with duck " << _host << " " << _port << "..." << std::endl;
	std::cout << "\033[s" << PROMPT;
	fflush(stdout);
}

Clt::~Clt() {
	/* destroy the client */
	close(_fd);
}

void	Clt::clientLoop( void ) {

	/* modified the console usage */
	tcgetattr(STDIN_FILENO, &_oldt);
	_newt = _oldt;
	_newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &_newt);

	while (true) {

		fd_set	fds;
		FD_ZERO(&fds);
		FD_SET(0, &fds); // check at input
		FD_SET(_fd, &fds);


		if (select(_fd + 1, &fds, NULL, NULL, &tv) < 0)
			break ;


		/* we can read from the input */
		if (FD_ISSET(0, &fds))
			this->treatChar();

		/* we can read from the server */
		if (FD_ISSET(_fd, &fds))
			if (!this->treatRequest())
				break ;
		fflush(stdout);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &_oldt);
	std::cout << "\033[uconnection closed by foreigned host" << std::endl;
}

void	Clt::treatChar(void) {

	char	c, buff[2] = { 0 };
	if (read(0, buff, 1) < 1)
		return ;

	c = buff[0];
	tcsetattr(STDIN_FILENO, TCSANOW, &_oldt);
	if (c == '\n') {
		_prompt += c;
		//this->removePrompt();
		std::cout << "\n\033[s" << PROMPT;
		if (_prompt.size() > 1) {
			send(_fd, (void *)_prompt.c_str(), _prompt.size(), 0);
		}
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

void	Clt::removePrompt(void) { std::cout << "\033[u\033[J"; }

void	Clt::do_backspace(void) {

	if (_prompt.size() <= 0)
		return ;
	_prompt = _prompt.substr(0, _prompt.size() - 1);
	this->removePrompt();
	std::cout << PROMPT << _prompt;
}

bool	Clt::treatRequest(void) {

	char	buff[BUFFER_SIZE] = { 0 };
	ssize_t	rd = -1;

	if ((rd = recv(_fd, buff, BUFFER_SIZE, 0)) < 1)
		return false;

	this->removePrompt();
	std::cout << buff << "\033[s";
	std::cout << PROMPT << _prompt;
	return true;
}