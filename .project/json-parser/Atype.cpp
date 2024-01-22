#include "Atype.hpp"

namespace JSON {

	Atype::Atype( void ) {}

	Atype::Atype( const std::string &name, std::string rawjson ) 
	: _name(name), _rawjson(rawjson) {
		// trim(_rawjson);
	}

	Atype::~Atype( void ) {}
}