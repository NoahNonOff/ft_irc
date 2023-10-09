// client.hpp
//
// Author: Noah BEAUFILS
// Date: 9-oct-2023

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Server;

class Client
{
	private:
		int const 			_fd;

		std::string			_username; /* whatever you want */
		std::string			_nickname; /* unique across the server */

		bool				_validated; /* does the client enter the password */
		short				_validateTry; /* remainning tries */

		bool				_is_msg;
		std::string			_msg_to_send;

		Channel				*_channel; /* actual channel */

	public:
		Client( int, std::string const & );
		~Client();

		int const &getFd( void ) const;
		std::string const &getUsername( void ) const;
		std::string const &getNickname( void ) const;
		std::string const &getMsg( void ) const;
		bool const &getValidation( void ) const;
		bool const &getWritting( void ) const;

		void	setWritting( bool );
		void	setValidation( bool );
		void	setMsg( std::string const & );
		void	addMsg( std::string const & );

		bool	treatRequest( std::string const &, Server * );
		bool	secure_connection( std::string const &, std::string const & );
		bool	executeCommand( std::string const &, Server * );
		void	launchMessage(std::string const &request);

		void	userCMD( void );
		void	helpCMD( void );
};

#endif