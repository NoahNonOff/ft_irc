#pragma once

#include "Atype.hpp"
#include "Parser.hpp"

namespace JSON {

	class Array : public Atype {

		private:
			// to do

		public:
			Array( void );
			Array( const std::string & );
			~Array( void );

			void	parse( void );
	};
}