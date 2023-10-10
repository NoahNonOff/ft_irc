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

	_msg = "";
	_prompt = "";
	_toRead = false;

	system("clear"); // clear the console
	std::cout << "\t\e[36m\e[1m[ ==== IRC Client ==== ]\e[0m\n" << std::endl;
	std::cout << "Connecting with duck " << _host << " " << _port << "..." << std::endl;
}

Clt::~Clt() {
	/* destroy the client */
	close(_fd);
}

// use select to non-block the read
// create a thread to check if a msg is received
void	Clt::clientLoop( void ) {

	char	c;
	while (true) {

		c = clt_extract_char();
		if (c == '\n') {
			std::cout << _prompt << std::endl; // send(_prompt)
			_prompt = "";
		}
		else
			_prompt += c;
	}
}

/*
void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}
*/