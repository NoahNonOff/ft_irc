#include "String.hpp"

namespace JSON {

	String::String(void) : Atype(e_string, "") {}

	String::String(const std::string &rawjson) : Atype(e_string, rawjson) {}

	String::~String(void) {}

	const std::string	&String::getStr(void) const { return _str; }

	void	String::parse(void) {

		const std::string	&raw = getRaw();
		const size_t	len = raw.length();

		for (size_t i = 0; i < len; i++)
			if (raw[i] != '\"' || (i && !isEscChar(raw, i, '\"')))
				_str += raw[i];
	}
}