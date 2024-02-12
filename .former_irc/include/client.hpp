// client.hpp
//
// Author: Noah BEAUFILS
// Date: 15-oct-2023

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Server;
class Channel;

class Client
{
	private:
		int const 					_fd;

		std::string					_username; /* whatever you want */
		std::string					_nickname; /* unique across the server */

		bool						_validated; /* does the client enter the password */
		short						_validateTry; /* remainning tries */

		bool						_is_msg;
		std::string					_msg_to_send;

		Channel						*_channel; /* actual channel */
		bool						_channelAccess;
		std::vector<std::string>	_invitation;

	public:
		Client( int, std::string const & );
		~Client();

		int const &getFd( void ) const;
		Channel *getChannel( void ) const;
		std::string const &getUsername( void ) const;
		std::string const &getNickname( void ) const;
		std::string const &getMsg( void ) const;
		bool const &getValidation( void ) const;
		bool const &getWritting( void ) const;

		void	setWritting( bool );
		void	setValidation( bool );
		void	setChannel( Channel * );
		void	setMsg( std::string const & );
		void	addMsg( std::string const & );
		void	setMp(std::string const &msg, std::string const &src);

		bool	treatRequest( std::string const &, Server * );
		void	accessToChannel( std::string const & );
		bool	secure_connection( std::string const &, std::string const & );
		bool	executeCommand( std::string const &, Server * );
		void	launchMessage(std::string const &request);
		void	quitChannel( void );

		/* Commands functions */
		void	pingCMD( void );
		void	userCMD( void );
		void	usersCMD( void );
		void	helpCMD( void );
		void	partCMD( void );
		void	listCMD( Server * );
		void	nameCMD( std::vector<std::string> );
		void	kickCMD( std::vector<std::string> );
		void	modeCMD( std::vector<std::string> );
		void	topicCMD( std::vector<std::string> );
		void	nickCMD( std::vector<std::string>, Server * );
		void	chatCMD( std::vector<std::string>, Server * );
		void	joinCMD( std::vector<std::string>, Server * );
		void	inviteCMD( std::vector<std::string>, Server * );

		void	createChannel( std::string const &, Server * );
		void	joinChannel( Channel * );
		bool	isInvite( std::string const & );
		void	invite( std::string const & );
};

#endif