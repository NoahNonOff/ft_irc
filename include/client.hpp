#pragma once

#include "irc.hpp"

class Server;

class Client
{
	public:
		typedef std::vector<std::string>	arg_array; /* array of arguments */

	private:
		const int 		_fd;

		std::string		_username;	/* whatever you want */
		std::string		_nickname;	/* unique across the server */

		bool			_validated;	/* Is the client authorized ? */

		std::string		_msg;		/* message to send */

	public:
		Client( const int );
		~Client( void );

		bool	treatRequest( std::string const &, Server * );

		const int 			&getFd( void ) const;
		const std::string	&getUsername( void ) const;
		const std::string	&getNickname( void ) const;
		const std::string	&getMsg( void ) const;

		bool	hasMsg( void );
		void	setMsg( const std::string & );


	private:
		bool	executeCommand( std::string const &, Server * );

		void 	capCMD( arg_array );
		void 	pingCMD( arg_array );
};
