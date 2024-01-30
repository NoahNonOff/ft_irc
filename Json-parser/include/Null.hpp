#pragma once

#include "Atype.hpp"
#include "Parser.hpp"

namespace JSON {

	class Null : public Atype {

		public:
			Null( void );
			Null( const std::string & );
			~Null( void );

			void	parse( void );
	};
}