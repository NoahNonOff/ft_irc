#pragma once

#include <iostream>

namespace JSON {

	enum type { e_object, e_array, e_string, e_number, e_boolen, e_null };

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

			const type &getType( void ) const;
			const std::string &getRaw( void ) const;
			const std::string &getKey( void ) const;

			std::string &getRaw( void );

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