// clt.hpp
//
// Author: Noah BEAUFILS
// Date: 10-oct-2023

#ifndef CLT_HPP
# define CLT_HPP

/* ============== include ============== */
# include <iostream>
# include <string>
# include <cstdio>
# include <cstdlib> /* exit */
# include <csignal> /* signal */

# include <unistd.h>
# include <netinet/in.h> /* htons() */
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <strings.h> // bzero()
# include <termios.h> // termios

/* =============== macro =============== */
# define SA struct sockaddr
# define PROMPT "\x1B[1m$> \x1B[0m"

/* =============== class =============== */
class Clt
{
	private:
		std::string const	_host;
		int const			_port;
		int					_fd;
		std::string			_prompt;

		bool				_toRead; /* true if a new message is received */
		std::string			_msg; /* the mesage received */

	public:
		Clt( std::string const &, int );
		~Clt();

		void	clientLoop( void );
};

/* =============== proto =============== */
char	clt_extract_char( void );

#endif