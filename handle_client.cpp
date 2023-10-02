// handle_client.cpp
//
// Author: Noah BEAUFILS
// Date: 2-oct-2023

#include "irc.hpp"

/* a message is always terminate by '\n' */

/* --------------------------- handle client --------------------------- */
void	*handle_client(void	*arg) {

	Client	*client = (Client *)arg;

	/* read a message from the connection */
	char	buffer[BUFFER_SIZE] = {0};
	ssize_t	bytes = read(client->getFd(), buffer, BUFFER_SIZE - 1);
	buffer[bytes] = 0;

	std::cout << "msg: __\"" << buffer << "\"__" << std::endl;

	/* create a response */
	std::string	response = "This is the response\n";
	send(client->getFd(), response.c_str(), response.size(), 0);

	/* closed the connection */
	delete client;
	return NULL;
}