// server.hpp
//
// Author: Noah BEAUFILS
// Date: 8-oct-2023

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

class Client;

class Server
{
	private:
		std::string const		_password; /* password of the server */
		int						_fd;
		int						_numClient;
		struct sockaddr_in		_sockAddr;
		std::map<int, Client *>	_clients;

		fd_set					_readfds;
		fd_set					_writefds;

		// channel's list [vector]

	public:
		Server( int, std::string const & );
		~Server();

		int const &getFd( void ) const;
		std::string const &getPassword( void ) const;
		std::map<int, Client *> const &getClients( void ) const;

		void	run( void );
		void	addClient( void );
		bool	readFromClient( int );
		void	sendToClient( int );

		class	init_error : public std::exception {
			private:
				char	*msg;
			public:
				init_error(char *message) : msg(message) {}
				virtual const char* what() const throw () { return msg; }
		};

		class	run_error : public std::exception {
			private:
				char	*msg;
			public:
				run_error(char *message) : msg(message) {}
				virtual const char* what() const throw () { return msg; }
		};
};

#endif