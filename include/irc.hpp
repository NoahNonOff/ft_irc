#pragma once

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

#include "Parser.hpp"
#include "Sha256.hpp"
#include "Md5.hpp"

#include "request.hpp"
#include "server.hpp"
#include "client.hpp"

//////////////// Socket ///////////////
/* [ "https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/" ] */
/* [ "http://vidalc.chez.com/lf/socket.html#lowlevel" ] */

///////////////// RFC /////////////////
/* ["https://modern.ircdocs.horse/"] */
/* [ "https://www.rfc-editor.org/rfc/rfc2812" ] */

///////////////// Port ////////////////
/* ["https://www.beyondtrust.com/docs/privilege-management/unix-linux/admin/settings/port-usage.htm"] */

//////////////////////////////////////////////////////

#define BUFFER_SIZE 513 // 510 + 2 + 1 (max message size + CR-LF pair + '\0')
#define to_str( s ) # s

/* connection to the server via IRSSI "/connect -4 127.0.0.1 4434" */

int							stoi( std::string const & );
std::string					mtos( char * );
std::vector<std::string>	splitCmds( std::string const );
std::string					toLower(std::string const &str);

