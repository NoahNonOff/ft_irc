#include "parser.hpp"

#define EXTLEN 5
#define EXT ".json"

namespace JSON {

	inline bool	isCorrectExt(const std::string &filename) {

		int	fileLen = filename.length();
		if (EXTLEN >= fileLen)
			return false;
		return (filename.substr(fileLen - EXTLEN, EXTLEN) == EXT);
	}

	inline bool	fileExists(const std::string &filename) {

		struct stat	info;
		return (!stat(filename.c_str(), &info)); 
	}

	Object	*parseFile(const std::string &filename) {

		if (!isCorrectExt(filename))
			throw std::runtime_error("Error: File extension is not .json");

		else if (!fileExists(filename))
			throw std::runtime_error("Error: File " + filename + " does not exist");

		std::ifstream in(filename.c_str());
		if (!in.is_open())
			throw std::runtime_error("Error: Cannot open file " + filename);

		std::string raw = std::string(std::istreambuf_iterator<char>(in),
			std::istreambuf_iterator<char>());

		return parseRaw(raw);
	}

	Object	*parseRaw(std::string &raw) {

		Object *ret = new Object(raw);

		if (!ret)
			throw std::runtime_error("Object: Allocation failed");

		try {
			ret->parse();
		} catch (std::exception &e) {
			if (ret)
				delete ret;
			throw std::runtime_error(e.what());
		}
		return ret;
	}

}

/* -------------------- utils --------------------*/

void	skipWhitespaces(const std::string &str, size_t &i) {

	const size_t len = str.length();
	for (; i < len && std::isspace(str[i]); i++)
		;;
}

// false -> escaped char | true -> non-escaped char
bool	isEscChar(const std::string &str, size_t i, char c) {

	return (str[i] == c && (i == 0 || str[i - 1] != '\\'));
}

static inline std::string	&rtrim(std::string &str, const char *garbage) {

	str.erase(str.find_last_not_of(garbage) + 1);
	return str;
}

static inline std::string	&ltrim(std::string &str, const char *garbage) {

	str.erase(0, str.find_first_not_of(garbage));
	return str;
}

void	trim(std::string &str, const char *garbage) {

	ltrim(rtrim(str, garbage), garbage);
}