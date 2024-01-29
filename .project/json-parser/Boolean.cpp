#include "Boolean.hpp"

namespace JSON {

	Boolean::Boolean(void) : Atype(e_boolean, "") {}

	Boolean::Boolean(const std::string &rawjson) : Atype(e_boolean, rawjson) {}

	Boolean::~Boolean(void) {}

	const bool	&Boolean::getVal(void) const { return _val; }

	void	Boolean::parse(void) {

		if (!getRaw().compare("true"))
			_val = true;
		else if (!getRaw().compare("false"))
			_val = false;
		else
			throw Atype::parseException("Boolean: cannot convert " + getRaw());
	}
}