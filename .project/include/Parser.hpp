#pragma once

#include <iostream> /* debuggng */
#include <fstream> /* ifstream */
#include <sys/stat.h> /* stat */

#include <map>
#include <algorithm>

#include "Atype.hpp"
#include "Object.hpp"
#include "String.hpp"

/* --------------------------------------------------- */

namespace JSON {

	class Object;

	Object	*parseFile( const std::string & );
	Object	*parseRaw( std::string & );

	bool	isCorrectExt( const std::string & );
	bool	fileExists( const std::string & );
}

/* utils */
void	skipWhitespaces( const std::string &, size_t & );
bool	isEscChar( const std::string &, size_t , char );