#include "Object.hpp"

namespace JSON {

	Object::Object(void) : Atype(e_object, "") {}

	Object::Object(const std::string &rawjson) : Atype(e_object, rawjson) {}

	Object::~Object(void) {

		for (iterator it = _map.begin(); it != _map.end(); it++)
			if (it->second)
				delete it->second;

		_map.clear();
	}

	Object::iterator		Object::begin(void) { return _map.begin(); }
	Object::iterator		Object::end(void) { return _map.end(); }
	Object::const_iterator	Object::begin(void) const { return _map.begin(); }
	Object::const_iterator	Object::end(void) const { return _map.end(); }

	Atype	*Object::operator[](std::string const &key) const {

		for (const_iterator it = _map.begin(); it != _map.end() ;it++) {
			if (!it->first.compare(key))
				return it->second;
		}
		return NULL;
	}

	void	Object::removeBraces(void) {

		std::string	&raw = getRaw();
		size_t		len = raw.length();

		if (!isEscChar(raw, 0, '{'))
			throw Atype::parseException("Object: Beginning must be non-escaped \"{\"");
		else if (!isEscChar(raw, len - 1, '}'))
			throw Atype::parseException("Object: End must be non-escaped \"{\"");
		raw.erase(len - 1, 1); // remove the last brace
		raw.erase(0, 1); // remove the first brace
	}

	size_t	Object::countKeys(void) {

		size_t				ret = 0;
		const std::string	&raw = getRaw();
		const size_t		len = raw.length();

		struct Parse		info = { false, false, 0, 0 };

		for (size_t i = 0; i < len; i++) {
			switch (raw[i]) {
				case '{' :
					if (isEscChar(raw, i, '{'))
						info.inObject++;
					break ;
				case '}' :
					if (isEscChar(raw, i, '}'))
						info.inObject--;
					if (info.inObject < 0)
						throw Atype::parseException("Object: Unexpected end of curly brace: \"}\"");
					break ;
				case '[' :
					if (isEscChar(raw, i, '['))
						info.inArray++;
					break ;
				case ']' :
					if (isEscChar(raw, i, ']'))
						info.inArray--;
					if (info.inArray < 0)
						throw Atype::parseException("Object: Unexpected end of square brace: \"]\"");
					break ;
				case '\'' :
					if (isEscChar(raw, i, '\''))
						info.squote = !info.squote;
					break ;
				case '\"' :
					if (isEscChar(raw, i, '\"'))
						info.dquote = !info.dquote;
					break ;
				case ':' :
					if (isEscChar(raw, i, ':') && !info.inArray && !info.inObject &&
						!info.squote && !info.dquote)
						ret++;
					break ;
				default :
					break ;
			}
		}
		return ret;
	}

	std::string	Object::getRawKey(const std::string &raw, size_t &i) {

		if (!isEscChar(raw, i++, '\"'))
			throw Atype::parseException("Object: Key should start with non-escaped \"");

		size_t start = i;
		for (; i < raw.length() && raw[i] != '\"'; i++)
			;;

		if (!isEscChar(raw, i++, '\"'))
			throw Atype::parseException("Object:: Key should ends with non-escaped \"");

		if (i >= raw.length())
 			throw Atype::parseException("Object:: Unexpected EOF");

		return raw.substr(start, i - start - 1);
	}

	std::string	getRawValue(const std::string &raw, size_t &i) {

		struct Parse		info = { false, false, 0, 0 };
		size_t				start = i;

		for (; i < raw.length(); i++) {
			switch (raw[i]) {
				case '{' :
					if (isEscChar(raw, i, '{'))
						info.inObject++;
					break ;
				case '}' :
					if (isEscChar(raw, i, '}'))
						info.inObject--;
					break ;
				case '[' :
					if (isEscChar(raw, i, '['))
						info.inArray++;
					break ;
				case ']' :
					if (isEscChar(raw, i, ']'))
						info.inArray--;
					break ;
				case '\'' :
					if (isEscChar(raw, i, '\''))
						info.squote = !info.squote;
					break ;
				case '\"' :
					if (isEscChar(raw, i, '\"'))
						info.dquote = !info.dquote;
					break ;
				case ',' :
					if (!info.inArray && !info.inObject && !info.squote && !info.dquote && 
						isEscChar(raw, i - 1, ','))
						return raw.substr(start, i - start);
				case ' ' :
				case '\n' :
				case '\r' :
				case '\t' :
					if (!info.inArray && !info.inObject && !info.squote && !info.dquote)
						return raw.substr(start, i - start);
			}
		}
		if (info.inArray || info.inObject || info.squote || info.dquote)
			throw Atype::parseException("Object: Unclosed context detected");

		return raw.substr(start, raw.length() - start);
	}

	Atype	*Object::identify(const std::string &rawValue) {

		Atype	*ret = NULL;

		if ((rawValue[0] == '-' && isdigit(rawValue[1])) || (isdigit(rawValue[0])))
			ret = new JSON::Number(rawValue);
		else {
			switch (rawValue[0]) {
				case 't' :
				case 'f' :
					ret = new JSON::Boolean(rawValue);
					break ;
				case '{' :
					ret = new JSON::Object(rawValue);
					break ;
				case '[' :
					ret = new JSON::Array(rawValue);
					break ;
				case 'n' :
					ret = new JSON::Null(rawValue);
					break ;
				case '\"' :
					ret = new JSON::String(rawValue);
					break ;
				default :
					throw Atype::parseException("Object: Unknown value type");
			}
		}
		if (!ret)
			throw Atype::parseException("Object: Allocation failed");

		try {
			ret->parse();
		} catch (std::exception &e) {
			if (ret)
				delete ret;
			throw std::runtime_error(e.what());
		}
		return ret;
	}

	void	Object::extractPair(const std::string &raw, size_t &i, size_t &currentKey, const size_t &nbKeys) {

		Atype	*value = NULL;

		skipWhitespaces(raw, i);
		std::string	rawKey = getRawKey(raw, i);
		skipWhitespaces(raw, i);

		if (!isEscChar(raw, i++, ':'))
			throw Atype::parseException("Object: Key-value must be separated with non-escaped \":\"");

		skipWhitespaces(raw, i);
		std::string	rawValue = getRawValue(raw, i);
		skipWhitespaces(raw, i);

		value = identify(rawValue);
		if (!value)
			return ;
		value->setKey(rawKey);

		if (currentKey != nbKeys && !isEscChar(raw, i++, ',')) {
			if (value)
				delete value;
			throw Atype::parseException("Object: Key-value pairs must be separated with non-escaped \",\"");
		}

		for (iterator it = _map.begin(); it != _map.end() ;it++) {
			if (!it->first.compare(rawKey)) {
				if (!value)
					delete value;
				throw Atype::parseException("Object: Duplicated key \"" + rawKey + "\"");
			}
		}
		_map.insert(std::make_pair(rawKey, value));
	}

	void	Object::parse(void) {

		removeBraces();

		size_t	i = 0;
		const std::string	&raw = getRaw();
		const size_t		len = raw.length();
		const size_t		nbKeys = countKeys();

		for (size_t currentKey = 1; i < len && currentKey <= nbKeys; currentKey++)
			extractPair(raw, i, currentKey, nbKeys);
		skipWhitespaces(raw, i);
		if (raw[i])
			throw Atype::parseException("Object: Unexpected end, maybe \":\" was missed");
	}
}
