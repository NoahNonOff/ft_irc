#include "Atype.hpp"

void	trim(std::string &str, const char *garbage);

namespace JSON {

	Null	null;
	Object	nullobj;
	Array	nullarr;

	void	Atype::setKey(const std::string &key) { _key = key; }

	const type			&Atype::getType(void) const { return _type; }
	const std::string	&Atype::getKey(void) const { return _key; }
	const std::string	&Atype::getRaw(void) const { return _rawjson; }

	std::string			&Atype::getRaw(void) { return _rawjson; }

	Atype::Atype(void) {}

	Atype::Atype(const type &typeName, std::string rawjson) 
	: _type(typeName), _rawjson(rawjson) {

		(void)_type;
		trim(_rawjson, " \t\n\r");
	}

	Atype::~Atype(void) {}

	double	Atype::toNum(void) {

		JSON::Number *t1 = dynamic_cast<JSON::Number *>(this);
		if (t1)
			return t1->getNum();

		JSON::Boolean *t2 = dynamic_cast<JSON::Boolean *>(this);
		if (t2)
			return t2->getVal();

		return std::strtod("nan", NULL);
	}

	std::string	Atype::toString(void)  {

		JSON::String *t1 = dynamic_cast<JSON::String *>(this);
		if (t1)
			return t1->getStr();
		
		JSON::Null *t2 = dynamic_cast<JSON::Null *>(this);
		if (t2)
			return "Null";
		
		JSON::Number *t3 = dynamic_cast<JSON::Number *>(this);
		if (t3)
			return t3->getRaw();
		
		JSON::Boolean *t4 = dynamic_cast<JSON::Boolean *>(this);
		if (t4)
			return t4->getVal() ? "true" : "false";

		JSON::Object *t5 = dynamic_cast<JSON::Object *>(this);
		if (t5)
			return "{...}";

		JSON::Array *t6 = dynamic_cast<JSON::Array *>(this);
		if (t6)
			return "[...]";

		return "unknown";
	}

	Object	*Atype::toObject(void) {

		JSON::Object *t1 = dynamic_cast<JSON::Object *>(this);
		if (t1)
			return t1;
		return &nullobj;
	}

	Array	*Atype::toArray(void) {

		JSON::Array *t1 = dynamic_cast<JSON::Array *>(this);
		if (t1)
			return t1;
		return &nullarr;
	}

	bool Atype::toBool(void) {

		JSON::Boolean *t1 = dynamic_cast<JSON::Boolean *>(this);
	
		if (t1)
			return t1->getVal();
		return false;
	}


	Atype	*Atype::get(const std::string &key) {

		JSON::Object *t1 = dynamic_cast<JSON::Object *>(this);

		if (t1)
			return t1[key];
		return &null;
	}

	Atype	*Atype::get(const size_t index) {

		JSON::Array *t1 = dynamic_cast<JSON::Array *>(this);

		if (t1)
			return t1[index];
		return &null;
	}

	bool	Atype::isNum(void) const { return (dynamic_cast<JSON::Number *>(this) != NULL); }

	bool	Atype::isStr(void) const { return (dynamic_cast<JSON::String *>(this) != NULL); }

	bool	Atype::isBool(void) const { return (dynamic_cast<JSON::Boolean *>(this) != NULL); }

	bool	Atype::isArr(void) const { return (dynamic_cast<JSON::Array *>(this) != NULL); }

	bool	Atype::isObj(void) const { return (dynamic_cast<JSON::Object *>(this) != NULL); }

	bool	Atype::isNull(void) const {

		JSON::Array *t1 = dynamic_cast<JSON::Null *>(this) != NULL;

		if (this == &null || this == &nullobj || this == &nullarr || t1)
			return true;
		return false;
	}
}