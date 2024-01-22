# include "Object.hpp"

namespace JSON {

	Object::Object( void ) {}

	Object::~Object( void ) {

		for (iterator it = _map.begin(); it != _map.end(); it++)
			if (it->second)
				delete it->second;

		_map.clear();
	}
}