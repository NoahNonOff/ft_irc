// irc.hpp
//
// Author: Noah BEAUFILS
// Date: 8-oct-2023

#ifndef IRC_HPP
# define IRC_HPP

/* -------------------- library -------------------- */

# include <iostream>
# include <string>
# include <exception>
# include <cstdlib> /* exit */
# include <csignal> /* signal */
# include <cerrno> /* errno */
# include <fstream>
# include <vector>
# include <map>

# include <stdlib.h>
# include <unistd.h>
# include <netinet/in.h> /* htons() */
# include <sys/socket.h>
# include <string.h> /* strcmp */
# include <arpa/inet.h>
# include <netinet/in.h>

# include "channel.hpp"
# include "server.hpp"
# include "client.hpp"

/* --------------------- macros -------------------- */

# ifndef MAX_WAIT
#  define MAX_WAIT 7
# endif

# ifndef MAX_CLIENT
#  define MAX_CLIENT 10
# endif

# define PROMPT "\x1B[1m$> \x1B[0m"
# define BUFFER_SIZE 1000
# define to_str( s ) # s

static const std::string	lst_name[10] = {
	"user0",
	"user1",
	"user2",
	"user3",
	"user4",
	"user5",
	"user6",
	"user7",
	"user8",
	"user9"
};

/* -------------------- utils --------------------- */

int				_stoi( std::string const & );
std::string		_mtos( char * );
t_roles			createT_roles( bool );

/* ------------------- commands ------------------- */
std::vector<std::string>	splitCmds(std::string const command);
bool						is_request(std::string const &request);

#endif