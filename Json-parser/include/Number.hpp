#pragma once

#include "Atype.hpp"
#include "Parser.hpp"

namespace JSON {

	class Number : public Atype {

		private:
			double	_num;

		public:
			Number( void );
			Number( const std::string & );
			~Number( void );

			const double	&getNum( void ) const;

			void	parse( void );
	};
}