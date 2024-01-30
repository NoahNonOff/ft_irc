#pragma once

#include <iostream> /* debuggng */
#include <fstream> /* ifstream */
#include <sys/stat.h> /* stat */

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "Atype.hpp"

/* --------------------------------------------------- */

namespace JSON {

	class Object;
	class Array;
	class String;
	class Boolean;
	class Number;
	class Null;

	Object	*parseFile( const std::string & );
	Object	*parseRaw( std::string & );

	bool	isCorrectExt( const std::string & );
	bool	fileExists( const std::string & );
}

/* utils */
void	skipWhitespaces( const std::string &, size_t & );
bool	isEscChar( const std::string &, size_t , char );