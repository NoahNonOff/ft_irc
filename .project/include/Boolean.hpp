#pragma once

#include "Atype.hpp"
#include "Parser.hpp"

namespace JSON {

	class Boolean : public Atype {

		private:
			bool	_val;

		public:
			Boolean( void );
			Boolean( const std::string & );
			~Boolean( void );

			const bool	&getVal( void ) const;

			void	parse( void );
	};
}