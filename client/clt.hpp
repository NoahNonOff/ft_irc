// clt.hpp
//
// Author: Noah BEAUFILS
// Date: 10-oct-2023

#ifndef CLT_HPP
# define CLT_HPP

/* ============== include ============== */
# include <iostream>
# include <string>
# include <cstdlib> /* exit */
# include <csignal> /* signal */

# include <unistd.h>
# include <netinet/in.h> /* htons() */
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <strings.h> // bzero()

/* =============== macro =============== */
# define SA struct sockaddr

/* =============== class =============== */
class Clt
{
	private:
		std::string const	_host;
		int const			_port;
		int					_fd;

	public:
		Clt( std::string const &, int );
		~Clt();

		void	clientLoop( void );
};

/* =============== proto =============== */

#endif