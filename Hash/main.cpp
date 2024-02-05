#include "Sha256.hpp"

int	main(int ac, char **av) {

	if (ac != 2) {

		std::cerr << "./test <to_hash>" << std::endl;
		return 1;
	}
	try {

		const std::string	&hash = sha256(av[1]);
		std::cout << "sha256: \"" << hash << "\"" << std::endl;
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}