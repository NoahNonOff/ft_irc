#include "Number.hpp"

namespace JSON {

	Number::Number(void) : Atype(e_number, "") {}

	Number::Number(const std::string &rawjson) : Atype(e_number, rawjson) {}

	Number::~Number(void) {}

	const double	&Number::getNum(void) const { return _num; }

	void	Number::parse(void) {

		try {

			const string &raw = getRaw();
			_num = std::stod(raw);
		}
		catch (std::exception &e) {
			throw AType::ParseException("Number: [" + e.what() + "] cannot convert " + raw);
		}
	}
}