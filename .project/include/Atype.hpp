#pragma once

#include <iostream>

namespace JSON {

	class Object;
	class Array;

	typedef struct Parse {
		bool	dquote;
		bool	squote;
		int		inArray;
		int		inObject;
	}	Parse;

	enum type { e_object, e_array, e_string, e_number, e_boolean, e_null };

	class Atype
	{
		private:
			type		_type;
			std::string	_key;
			std::string	_rawjson;

		public:
			Atype( void );
			Atype( const type &, std::string );
			virtual ~Atype( void );

			void	setKey( const std::string & );

			const type			&getType( void ) const;
			const std::string	&getRaw( void ) const;
			const std::string	&getKey( void ) const;

			std::string			&getRaw( void );

			/* -------------------------------- */

			double	toNum( void );
			std::string	toString( void );
			Object	*toObject( void );
			Array	*toArray( void );
			bool	toBool( void );

			Atype	*get( const std::string & );
			Atype	*get( const size_t );

			bool	isNum( void ) const;
			bool	isStr( void ) const;
			bool	isBool( void ) const;
			bool	isArr( void ) const;
			bool	isObj( void ) const;
			bool	isNull( void ) const;

			virtual void parse( void ) = 0;

			//Exceptions
			class parseException : public std::exception {
				private:
					std::string	_msg;
				public:
					parseException(std::string message) : _msg(message) {}
					virtual ~parseException() throw() {}
					virtual const char *what(void) const throw() { return _msg.c_str(); }
			};
	};
}

#include "Array.hpp"
#include "Object.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include "Number.hpp"
#include "Null.hpp"