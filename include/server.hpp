#pragma once

#include "irc.hpp"

typedef struct s_fdSet {
	fd_set	readfds;
	fd_set	writefds;
}	fdSet;

class Client;

class Server
{
	public:
		typedef std::map<int, Client *>::iterator	clt_iterator;

	private:
		const int				_pHash;			/* sha256 = 0 / md5 = 1 */
		const std::string		_password;		/* password of the server */

		int						_numClient;		/* Actual number of client */
		int						_port;
		int						_fd;

		int						_maxWait;		/* max pending connections */
		int						_maxClts;		/* max number of clients */
		int						_maxChannel;	/* max number of channels */

		struct sockaddr_in		_sockAddr;
		std::map<int, Client *>	_clients;

		fdSet					_fds;

	public:
		Server( int, std::string const &, int );
		~Server( void );

		const int &getFd( void ) const;
		const int &getPort( void ) const;
		const std::string &getPassword( void ) const;
		const std::map<int, Client *> &getClients( void ) const;

		void	init( void );
		void	getConfig( const std::string & );
		void	run( void );

		class	serverError : public std::exception {
			private:
				char	*_msg;
			public:
				serverError(char *message) : _msg(message) {}
				virtual const char* what() const throw () { return _msg; }
		};

	private:
		void	addClient( void );
		bool	removeClient( int );
		bool	readFromClient( int );
		void	sendToClient( int );
};
