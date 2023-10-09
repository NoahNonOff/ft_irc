// channel.hpp
//
// Author: Noah BEAUFILS
// Date: 9-oct-2023

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Client;

/* --------------------- struct -------------------- */
typedef struct {
	bool	admin;
}	t_roles;

/* ---------------------- class -------------------- */
class Channel
{
	private:
		std::string					_name;
		std::string					_password; /* password of the channel */

		std::map<Client *, t_roles>	_roles;

	public:
		Channel( std::string const & );
		~Channel();

		std::string	const &getName( void ) const;
		std::string	const &getPassword( void ) const;
};

#endif