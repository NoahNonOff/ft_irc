// channel.hpp
//
// Author: Noah BEAUFILS
// Date: 13-oct-2023

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Client;

/* ---------------------- class -------------------- */
class Channel
{
	private:
		std::string					_name;

		std::string					_topic;
		std::string					_password; /* password of the channel */
		int							_userLimit;

		bool						_inviteOnly;

		std::map<Client *, bool>	_admin;

	public:
		Channel( std::string const & );
		~Channel();

		std::string	const &getName( void ) const;
		std::string	const &getTopic( void ) const;
		std::string	const &getPassword( void ) const;
		std::map<Client *, bool> const &getAdmin( void ) const;

		void	setTopic( std::string const & );

		void	kickUser( Client * );
		void	removeUser( Client * );
		void	addUser( Client *, bool );
		void	broadcast( std::string const &, Client * );
		bool	isAdmin( Client * );
		Client	*isInChannel( std::string const & );
};

#endif