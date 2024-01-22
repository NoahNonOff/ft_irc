#pragma once

#include <iostream>

namespace JSON {

	class Object;

	class Atype
	{
		private:
			std::string	_key;
			std::string	_name;
			std::string	_rawjson;

		public:
			Atype( void );
			Atype( const std::string &, std::string );
			virtual ~Atype( void );

			virtual void parse( void ) = 0;
	};
}