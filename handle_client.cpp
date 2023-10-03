// handle_client.cpp
//
// Author: Noah BEAUFILS
// Date: 2-oct-2023

#include "irc.hpp"

bool	secure_connection(Client *client) {

	short		lives = 3;
	std::string	question = "$>please enter the password: ";

	while (lives > 0) {

		char		buffer[BUFFER_SIZE] = {0};
		send(client->getFd(), question.c_str(), question.size(), 0);
		ssize_t	bytes = read(client->getFd(), buffer, BUFFER_SIZE);
		buffer[bytes - 2] = 0;

		std::string answer = buffer;
		if (!client->getPassword().compare(answer))
			break ;
		lives--;
		question = "$>bad password, please try again: ";
	}
	if (!lives)
		return false;
	return true;
}

/* --------------------------- handle client --------------------------- */
void	*handle_client(void	*arg) {

	Client	*client = (Client *)arg;

	if (!secure_connection(client)) {
		std::cout << "client " << client->getFd() << ": was ejected (because of password error)" << std::endl;
		delete client;
		return NULL;
	}

	/* create a response */
	std::string	response = "You are connected\n";
	send(client->getFd(), response.c_str(), response.size(), 0);

	/* closed the connection */
	delete client;
	return NULL;
}