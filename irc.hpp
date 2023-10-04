// irc.hpp
//
// Author: Noah BEAUFILS
// Date: 4-oct-2023

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
# include <string.h> /* strcmp */
# include <map>

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
		int const 			_fd;
		std::string	const	_password;
		std::string			_username;

	public:
		Client( int, std::string const & );
		~Client();

		int const &getFd( void ) const;
		std::string const &getPassword( void ) const;
};

class Server
{
	private:
		std::string const		_password;
		int						_fd;
		struct sockaddr_in		_sockAddr;
		std::map<int, Client *>	_clients;

	public:
		Server( int, std::string const & );
		~Server();

		int const &getFd( void ) const;
		std::map<int, Client *> const &getClients( void ) const;
};

/* ------------------- prototype ------------------- */

int		_stoi( std::string const & );

#endif