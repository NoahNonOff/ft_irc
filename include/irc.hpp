#pragma once

// TO-DO (02/18/2024)
//
//		-> fix bugs when connecting
//	Ok	-> test the request parser (https://modern.ircdocs.horse/#parameters)
//		-> create the class Channel
//

//////////////// Socket ///////////////
/* [ "https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/" ] */
/* [ "http://vidalc.chez.com/lf/socket.html#lowlevel" ] */

///////////////// RFC /////////////////
/* ["https://modern.ircdocs.horse/"] */
/* [ "https://www.rfc-editor.org/rfc/rfc2812" ] */

///////////////// Port ////////////////
/* ["https://www.beyondtrust.com/docs/privilege-management/unix-linux/admin/settings/port-usage.htm"] */

/* connection to the server via IRSSI "/connect -4 127.0.0.1 4434" */

//////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>			/* exit */
#include <csignal>			/* signal */
#include <cerrno>			/* errno */
#include <fstream>
#include <vector>
#include <map>

#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>		/* htons() */
#include <sys/socket.h>
#include <string.h>			/* strcmp */
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct s_serverInfo {
	std::string	name;
	std::string	version;
}	t_serverInfo;

#define BUFFER_SIZE 513 // 510 + 2 + 1 (max message size + CR-LF pair + '\0') + 4096 for tags
#define to_str( s ) # s

int							stoi( std::string const & );
std::string					mtos( char * );
std::string					toLower( std::string const & );

//////////////////////////////////////////////////////

/* ["https://www.alien.net.au/irc/irc2numerics.html"] */

#define RPL_YOURHOST			002

#define ERR_UNKNOWNCOMMAND		421
#define ERR_NONICKNAMEGIVEN		431
#define ERR_ERRONEUSNICKNAME	432
#define ERR_NICKNAMEINUSE		433
#define ERR_NOTREGISTERED		451
#define ERR_NEEDMOREPARAMS		461
#define ERR_ALREADYREGISTERED	462
#define ERR_PASSWDMISMATCH		464

//////////////////////////////////////////////////////

#include "Parser.hpp"
#include "Sha256.hpp"
#include "Md5.hpp"

#include "request.hpp"
#include "server.hpp"
#include "client.hpp"