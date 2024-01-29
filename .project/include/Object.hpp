#pragma once

#include "Atype.hpp"
#include "Parser.hpp"

namespace JSON {

	class Object : public Atype {

		public:
			typedef std::map<std::string, Atype *>::iterator iterator;
			typedef std::map<std::string, Atype *>::const_iterator const_iterator;
		
		private:
			std::map<std::string, Atype *> _map;

		public:
			Object( void );
			Object( const std::string & );
			~Object( void );

			void	parse( void );

			void	removeBraces( void );
			size_t	countKeys( void );
			void	extractPair( const std::string &, size_t &, size_t &, const size_t & );
			std::string	getRawKey( const std::string &, size_t & );
			Atype	*identify( const std::string & );
	};
}