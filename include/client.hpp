#pragma once

#include "irc.hpp"
#include <cstdarg>	/* variadic function */
#include <sstream>	/* stringstream */

class Server;
class Request;

#define F_PASS		0b00000001
#define F_NICK		0b00000010
#define F_USER		0b00000100
#define F_CAP		0b00001000

#define addFlag(A, B) ( (A) | (B) )
#define validated(X) ( (X) == 0b00001111 )

class Client
{
	private:
		const int 			_fd;
		const t_serverInfo	&_serverInfo;		/* All the needed info about the server */

		std::string			_username;			/* whatever you want */
		std::string			_nickname;			/* unique across the server */

		uint8_t				_registration;		/* Is the client authorized ? */

		std::string					_msg;		/* message to send */
		std::vector<std::string>	_toSend;

	public:
		Client( const int, const t_serverInfo & );
		~Client( void );

		void	treatRequest( std::string const &, Server *, bool & );

		const int 			&getFd( void ) const;
		const std::string	&getUsername( void ) const;
		const std::string	&getNickname( void ) const;
		const std::string	getMsg( void );

		bool	hasMsg( void ) const;

		void	err( const int , ... );
		void	rpl( const int , ... );

	private:

		void 	capCMD( Request );
		void	userCMD( Request );
		void	nickCMD( Request, Server * );
		void	passCMD( Request, Server * );

		void 	pingCMD( Request );
		void	quitCMD( Request, bool & );
};
