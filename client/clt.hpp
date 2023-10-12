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
# include <sys/select.h>
# include <sys/types.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <strings.h> // bzero()
# include <termios.h> // termios

/* =============== macro =============== */
# define SA struct sockaddr
# define PROMPT "\x1B[1m$> \x1B[0m"
# define PROMPT_SIZE 11
# define BUFFER_SIZE 10000

/* =============== class =============== */
class Clt
{
	private:
		std::string const	_host;
		int const			_port;
		int					_fd;
		std::string			_prompt;

		struct termios		_oldt;
		struct termios		_newt;

	public:
		Clt( std::string const &, int );
		~Clt();

		void	clientLoop( void );
		void	treatChar( void );
		void	do_backspace( void );
		void	removePrompt( void );
		bool	treatRequest( void );
};

/* =============== proto =============== */

#endif