#include "Sha256.hpp"

int	main(int ac, char **av) {

	if (ac == 1) {

		std::cerr << "./test <to_hash>" << std::endl;
		return 1;
	}
	try {

		for (int i = 1; i < ac; i++)
			std::cout << av[i] << ": \"" << HASH::sha256(av[i]) << "\"" << std::endl;
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}