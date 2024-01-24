#include "Atype.hpp"

void	trim(std::string &str, const char *garbage);

namespace JSON {

	void	Atype::setKey(const std::string &key) { _key = key; }

	const type &Atype::getType(void) const { return _type; }
	const std::string &Atype::getRaw(void) const { return _rawjson; }
	const std::string &Atype::getKey(void) const { return _key; }
	
	std::string &Atype::getRaw(void) { return _rawjson; }

	Atype::Atype(void) {}

	Atype::Atype(const type &typeName, std::string rawjson) 
	: _type(typeName), _rawjson(rawjson) {

		(void)_type;
		trim(_rawjson, " \t\n\r");
	}

	Atype::~Atype(void) {}
}