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
# include <cstdlib> /* exit */
# include <csignal> /* signal */
# include <cerrno> /* errno */
# include <map>

# include <unistd.h>
# include <netinet/in.h> /* htons() */
# include <sys/socket.h>
# include <string.h> /* strcmp */
# include <arpa/inet.h>
# include <netinet/in.h>

/* --------------------- macros -------------------- */

# ifndef MAX_WAIT
#  define MAX_WAIT 7
# endif

# ifndef MAX_CLIENT
#  define MAX_CLIENT 10
# endif

# define BUFFER_SIZE 1000
# define to_str( s ) # s

/* --------------------- class --------------------- */

class Client
{
	private:
		int const 			_fd;
		std::string			_username;

		bool				_validated; /* does the client enter the password */
		short				_validateTry; /* remainning tries */

		bool				_is_msg;
		std::string			_msg_to_send;

	public:
		Client( int );
		~Client();

		int const &getFd( void ) const;
		std::string const &getUsername( void ) const;
		bool const &getValidation( void ) const;
};

class Server
{
	private:
		std::string const		_password;
		int						_fd;
		struct sockaddr_in		_sockAddr;
		std::map<int, Client *>	_clients;

		fd_set					_readfds;

	public:
		Server( int, std::string const & );
		~Server();

		int const &getFd( void ) const;
		std::string const &getPassword( void ) const;
		std::map<int, Client *> const &getClients( void ) const;

		void	run( void );
		void	addClient( void );
		bool	clientRequest( int sd );

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

/* ------------------- prototype ------------------- */

int				_stoi( std::string const & );
std::string		_mtos( char * );

#endif