#include "Parser.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	try {
		JSON::Object	*json = JSON::parseFile(av[1]);

		std::cout << json->get("homeroomTeacher")->isObj() << std::endl;
		std::cout << json->get("homeroomTeacher")->get("lastName")->toString() << std::endl;
		std::cout << json->get("homeroomTeacher")->get("firstName") << std::endl;
		std::cout << json->get("homeroomTeacher") << std::endl;
		std::cout << json->get("phoneNumber")->get("firstName") << std::endl;
		if (!json->get("alpha")->get("test")->isNull())
			std::cout << "exist" << std::endl;
		else
			std::cout << "doesn't exist" << std::endl;

		delete json;
	} catch (std::exception &e) {
		std::cerr <<e.what() << std::endl;
	}
	return 0;
}