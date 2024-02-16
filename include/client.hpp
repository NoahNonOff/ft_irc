#pragma once

#include "irc.hpp"

class Server;
class Request;

class Client
{
	private:
		const int 		_fd;

		std::string		_username;	/* whatever you want */
		std::string		_nickname;	/* unique across the server */

		bool			_validated;	/* Is the client authorized ? */

		std::string		_msg;		/* message to send */

	public:
		Client( const int );
		~Client( void );

		void	treatRequest( std::string const &, Server *, bool & );

		const int 			&getFd( void ) const;
		const std::string	&getUsername( void ) const;
		const std::string	&getNickname( void ) const;
		const std::string	&getMsg( void ) const;

		bool	hasMsg( void );
		void	setMsg( const std::string & );


	private:

		void 	capCMD( void );
		void	nickCMD( Request );
		void	userCMD( Request );
		void	passCMD( Request );

		void 	pingCMD( Request );
		void	quitCMD( Request, bool & );
};
