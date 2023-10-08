// client.hpp
//
// Author: Noah BEAUFILS
// Date: 8-oct-2023

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Client
{
	private:
		int const 			_fd;
		std::string const	_password; /* password of the server */
		std::string			_username;

		bool				_validated; /* does the client enter the password */
		short				_validateTry; /* remainning tries */

		bool				_is_msg;
		std::string			_msg_to_send;

		// list of affiliated server [vector]

	public:
		Client( int, std::string const & );
		~Client();

		int const &getFd( void ) const;
		std::string const &getUsername( void ) const;
		std::string const &getMsg( void ) const;
		std::string const &getPassword( void ) const;
		bool const &getValidation( void ) const;
		bool const &getWritting( void ) const;

		void	setWritting( bool );
		void	setValidation( bool );
		void	setMsg( std::string const & );
		void	addMsg( std::string const & );

		bool	treatRequest( std::string const & );
		bool	secure_connection( std::string const & );
		bool	executeCommand( std::string const & );
};

#endif