#include "Number.hpp"

#include <stdlib.h>

namespace JSON {

	Number::Number(void) : Atype(e_number, "") {}

	Number::Number(const std::string &rawjson) : Atype(e_number, rawjson) {}

	Number::~Number(void) {}

	const double	&Number::getNum(void) const { return _num; }

	void	Number::parse(void) {

		const std::string	&raw = getRaw();
		char				*end = NULL;
		double				num = std::strtod(raw.c_str(), &end);

		std::string	e(end);
		if (e != "" && e != "f")
			throw Atype::parseException((std::string)"Number: cannot convert " + raw);

		_num = num;
	}
}