#include "parser.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	try {
		JSON::Object	*json = JSON::parseFile(av[1]);
		delete json;
	} catch (std::exception &e) {
		std::cerr <<e.what() << std::endl;
	}
	return 0;
}