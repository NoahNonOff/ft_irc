// server.hpp
//
// Author: Noah BEAUFILS
// Date: 14-oct-2023

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

class Client;
class Channel;

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

		std::vector<Channel *>	_channels;

	public:
		Server( int, std::string const & );
		~Server();

		int const &getFd( void ) const;
		std::string const &getPassword( void ) const;
		std::map<int, Client *> const &getClients( void ) const;
		std::vector<Channel *> const &getChannel( void ) const;

		void		run( void );
		void		addClient( void );
		bool		removeClient( int );
		bool		readFromClient( int );
		void		sendToClient( int );
		bool		isAlreadyTaken( std::string const & );
		Client		*isInServer( std::string const & );
		Channel		*addChannel( std::string const & );
		std::string	giveNickname( void );

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