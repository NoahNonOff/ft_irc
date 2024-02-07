#include "Sha256.hpp"
// #include "Md5.hpp"

int	main(int ac, char **av) {

	if (ac == 1) {

		std::cerr << "./test <to_hash>" << std::endl;
		return 1;
	}
	try {

		for (int i = 1; i < ac; i++) {
			std::cout << "[sha256]" << av[i] << ": \"" << hash::sha256(av[i]) << "\"" << std::endl;
			// std::cout << "[md5]" << av[i] << ": \"" << hash::md5(av[i]) << "\"" << std::endl;
			if (i < ac - 1)
				std::cout << "-------------------------------------------------------" << std::endl;
		}
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}