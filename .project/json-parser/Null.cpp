#include "Null.hpp"

namespace JSON {

	Null::Null(void) : Atype(e_null, "") {}

	Null::Null(const std::string &rawjson) : Atype(e_null, rawjson) {}

	Null::~Null(void) {}

	void	Null::parse(void) {

		if (getRaw().compare("null"))
			throw AType::ParseException("Null: failed to parse null");
	}
}