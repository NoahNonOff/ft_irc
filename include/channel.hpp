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

		bool						_topicRestriction;
		std::string					_topic;
		std::string					_password; /* password of the channel */
		int							_userLimit;

		bool						_inviteOnly;

		std::map<Client *, bool>	_admin;

	public:
		Channel( std::string const & );
		~Channel();

		bool const	&getTopicRestriction( void ) const;
		std::string const	&getName( void ) const;
		std::string	const	&getTopic( void ) const;
		std::string	const	&getPassword( void ) const;
		std::map<Client *, bool> const &getAdmin( void ) const;

		void	setTopic( std::string const & );

		void	kickUser( Client * );
		void	removeUser( Client * );
		bool	addUser( Client *, bool );
		void	broadcast( std::string const &, Client * );
		bool	isAdmin( Client * );
		Client	*isInChannel( std::string const & );

		/* options mode */
		void	mode_t( void );
		void	mode_o( std::vector<std::string>, Client * );
		void	mode_l( std::vector<std::string>, Client * );
};

#endif