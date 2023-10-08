// channel.hpp
//
// Author: Noah BEAUFILS
// Date: 8-oct-2023

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Channel
{
	private:
		std::string			_name;
		std::string const	_password; /* password of the channel */
		// list of users and roles [map<Client *, t_roles>]

	public:
		Channel( std::string const &, std::string const & );
		~Channel();

		std::string	const &getName( void ) const;
		std::string	const &getPassword( void ) const;
};

#endif