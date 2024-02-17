#pragma once

#include "irc.hpp"

class Request
{
	private:
		std::string					_prefix;	/* comment at the begining */
		std::string					_command;	/* the command */
		std::vector<std::string>	_arguments;	/* all the arguments after the command */
		std::vector<std::string>	_tags;		/* tags received with the command */

	public:
		Request( void );
		Request( const std::string & );
		~Request( void );

		size_t				size( void ) const;
		const std::string	&getPrefix( void ) const;
		const std::string	&getCommand( void ) const;

		bool				is( const std::string & ) const;
		bool				empty( void ) const;

		const std::string	&operator[]( const size_t & ) const;
};
