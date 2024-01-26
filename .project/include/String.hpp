#pragma once

#include "Atype.hpp"
#include "Parser.hpp"

namespace JSON {

	class String : public Atype {

		private:
			std::string	_str;

		public:
			String( void );
			String( const std::string & );
			~String( void );

			const std::string	&getStr( void ) const;

			void	parse( void );
	};
}