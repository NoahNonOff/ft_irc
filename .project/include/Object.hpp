#pragma once

#include "Atype.hpp"

namespace JSON {

	class Object : public Atype {

		public:
			typedef std::map<std::string, Atype *>::iterator iterator;
			typedef std::map<std::string, Atype *>::const_iterator const_iterator;
		
		private:
			std::map<std::string, Atype *> _map;

		public:
			Object( void );
			~Object( void );
	};
}