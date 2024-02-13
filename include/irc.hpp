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

#include "server.hpp"
#include "client.hpp"

/* [ "https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/" ] */
/* [ "https://www.cs.cmu.edu/~srini/15-441/S10/project1/pj1_description.pdf" ] */
/* [ "http://vidalc.chez.com/lf/socket.html#lowlevel" ] */
/* [ "https://www.rfc-editor.org/rfc/rfc2812" ] */

/* ["https://www.beyondtrust.com/docs/privilege-management/unix-linux/admin/settings/port-usage.htm"] */

#define BUFFER_SIZE 513 // 510 + 2 + 1 (max message size + CR-LF pair + '\0')
#define to_str( s ) # s

/* connection to the server via IRSSI "/connect -4 127.0.0.1 4434" */

//////////////////////////////////////////////////////

int							stoi( std::string const & );
std::string					mtos( char * );
std::vector<std::string>	splitCmds( std::string const );
std::string					toLower(std::string const &str);
