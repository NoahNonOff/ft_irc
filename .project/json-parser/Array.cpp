#include "Array.hpp"

namespace JSON {

	Array::Array(void) : Atype(e_array, "") {}

	Array::Array(const std::string &rawjson) : Atype(e_array, rawjson) {}

	Array::~Array(void) {

		for (iterator it = _arr.begin(); it != _arr.end(); it++)
			if (*it)
				delete *it;
		_arr.clear();
	}

	void	Array::removeBraces(void) {

		std::string		&raw = getRaw();
		const size_t	len = raw.length();

		if (!isEscChar(raw, 0, '['))
			throw Atype::parseException("Array: Beginning must be non-escaped \"[\"");
		else if (!isEscChar(raw, len - 1, ']'))
			throw Atype::parseException("Array: Ending must be non-escaped \"]\"");

		raw.erase(len - 1, 1);
		raw.erase(0, 1);
	}

	size_t	Array::countElem(void) {

		size_t				ret = 0;
		const std::string	&raw = getRaw();
		const size_t		len = raw.length();

		struct Parse		info = { false, false, 0, 0 };

		if (raw.find_first_not_of(' ') == std::string::npos)
			return 0;

		for (size_t i = 0; i < len; i++) {
			switch (raw[i]) {
				case '{':
					if (isEscChar(raw, i, '{'))
						info.inObject++;
					break;
				case '}':
					if (isEscChar(raw, i, '}'))
						info.inObject--;
					if (info.inObject < 0)
						throw Atype::parseException("Array: Unexpected end of curly brace: \"}\"");
					break;
				case '[':
					if (isEscChar(raw, i, '['))
						info.inArray++;
					break;
				case ']':
					if (isEscChar(raw, i, ']'))
						info.inArray--;
					if (info.inArray < 0)
						throw Atype::parseException("Array: Unexpected end of square brace: \"]\"");
					break;
				case '\'':
					if (isEscChar(raw, i, '\''))
						info.squote = !info.squote;
					break;
				case '\"':
					if (isEscChar(raw, i, '\"'))
						info.dquote = !info.dquote;
					break;
				case ',':
					if (isEscChar(raw, i, ',') && !info.inArray && !info.inObject &&
						!info.squote && !info.dquote)
						ret++;
					break;
				default:
					break;
			}
		}
		// ret++
		return ret;
	}

	std::string	Array::getRawElem(const std::string &raw, size_t &i) {

		struct Parse		info = { false, false, 0, 0 };
		size_t				start = i;

		for (; i < raw.length(); i++) {
			switch (raw[i]) {
				case '{':
					if (isEscChar(raw, i, '{'))
						info.inObject++;
					break;
				case '}':
					if (isEscChar(raw, i, '}'))
						info.inObject--;
					break;
				case '[':
					if (isEscChar(raw, i, '['))
						info.inArray++;
					break;
				case ']':
					if (isEscChar(raw, i, ']'))
						info.inArray--;
					break;
				case '\'':
					if (isEscChar(raw, i, '\''))
						info.squote = !info.squote;
					break;
				case '\"':
					if (isEscChar(raw, i, '\"'))
						info.dquote = !info.dquote;
					break;
				case ',':
					if (!info.inArray && !info.inObject && !info.squote && !info.dquote && 
						isEscChar(raw, i - 1, ',') )
						return raw.substr(start, i - start);
				case ' ':
				case '\n':
				case '\r':
				case '\t':
					if (!info.inArray && !info.inObject && !info.squote && !info.dquote)
						return raw.substr(start, i - start);
			}
		}
		if (info.inArray || info.inObject || info.squote || info.dquote)
			throw Atype::parseException("Array: Unclosed context detected");

		return raw.substr(start, raw.length() - start);
	}

	Atype	*Array::identify(std::string &raw) {

		Atype *ret = NULL;

		if ((raw[0] == '-' && isdigit(raw[1])) || (isdigit(raw[0])))
			ret = new JSON::Number(raw);
		else {
			switch (raw[0]) {
				case 't':
				case 'f':
					ret = new JSON::Boolean(raw);
					break ;
				case '{':
					ret = new JSON::Object(raw);
					break ;
				case '[':
					ret = new JSON::Array(raw);
					break ;
				case 'n':
					ret = new JSON::Null(raw);
					break ;
				case '\"':
					ret = new JSON::String(raw);
					break ;
				default:
					throw Atype::parseException("Array: Unknown value type");
			}
		}

		if (ret == NULL)
			throw std::runtime_error("Array: Allocation failed");

		try {
			ret->parse();
		} catch (std::exception &e) {
			delete ret;
			throw std::runtime_error((e.what()));
		}
		return ret;
	}

	void	Array::parse(void) {

		removeBraces();

		size_t				i = 0;
		const std::string	&raw = getRaw();
		const size_t		len = raw.length();
		const size_t		nbElem = countElem();

		for (size_t currentElem = 1; i < len && currentElem < nbElem; currentElem++) {

			skipWhitespaces(raw, i);
			std::string	rawElem = getRawElem(raw, i);

			skipWhitespaces(raw, i);
			Atype	*value = identify(rawElem);

			if (!isEscChar(raw, i++, ',')) {
				if (value)
					delete value;
				throw Atype::parseException("Array: Key-value pairs must be ended with non-escaped \",\"");
			}

			_arr.push_back(value);
		}
	}
}
