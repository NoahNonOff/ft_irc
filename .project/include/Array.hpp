#pragma once

#include "Atype.hpp"
#include "Parser.hpp"

namespace JSON {

	class Array : public Atype {

		private:
			typedef std::vector<Atype *>::iterator	iterator;
			typedef std::vector<Atype *>::const_iterator	const_iterator;

		private:
			std::vector<Atype *> _arr;

		public:
			Array( void );
			Array( const std::string & );
			~Array( void );

			void	parse( void );

			void			removeBraces( void );
			const size_t	countElem( void );
			string			getRawElem( const std::string &, size_t & );
			AType			*identify( string & );
	};
}