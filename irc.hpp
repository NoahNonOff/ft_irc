// irc.hpp
//
// Author: Noah BEAUFILS
// Date: 2-oct-2023

#ifndef IRC_HPP
# define IRC_HPP

/* -------------------- library -------------------- */

# include <iostream>
# include <string>
# include <exception>
# include <unistd.h>
# include <netinet/in.h> /* htons() */
# include <sys/socket.h>
# include <cstdlib> /* exit */
# include <csignal> /* signal */

/* --------------------- macros -------------------- */

# ifndef MAX_CONNECTION
#  define MAX_CONNECTION 10
# endif

# define BUFFER_SIZE 1000
# define to_str( s ) # s

/* --------------------- class --------------------- */

class Client
{
	private:
		int	_fd;
	public:
		Client(int clt_fd) : _fd(clt_fd) {}
		~Client() { close(_fd); }

		int const &getFd(void) const { return _fd; }
};

/* ------------------- prototype ------------------- */

int		_stoi(std::string const &str);
void	*handle_client(void *arg);

#endif

/* [ "for (char c : chaine)" ] */