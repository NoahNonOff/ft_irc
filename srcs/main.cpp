#include "irc.hpp"

Server *Server_ptr = NULL;

typedef struct {
	std::string	configFile;
	std::string	password;
	int			port;
	short		hash; // sha256 = 0 / md5 = 1
}	t_Args;

/////////////////////////////////////////////////////////////

void	endProg(int signal) {

	if (signal == SIGINT) {
		std::cout << "\033[2D"; /* remove the ^C */
		if (Server_ptr)
			delete Server_ptr;
		exit (0);
	}
}

/* ugly function to extract and verify all the flags */
static bool	extractFlag(int ac, char *av[], int &i, t_Args *v) {

	std::string	s(av[i]);

	if (!s.compare("--MD5")) {
		v->hash = 1;
	} else if (!s.compare("--SHA256")) {
		v->hash = 0;
	} else if (!s.compare("-h")) {
		if (i == ac - 1) {
			std::cerr << "PARSING: error: missing argument after \"-h\"" << std::endl;
			return 1;
		}
		s = av[++i];
		if (!toLower(s).compare("sha256")) {
			v->hash = 0;
		} else if (!toLower(s).compare("md5")) {
			v->hash = 0;
		} else {
			std::cerr << "PARSING: error: unknown argument: " << s << std::endl;
			return 1;
		}
	} else if (!s.compare("-c")) {
		if (i == ac - 1) {
			std::cerr << "PARSING: error: missing argument after \"-c\"" << std::endl;
			return 1;
		}
		v->configFile = av[++i];
	} else {
		std::cerr << "PARSING: error: unknown flag: " << s << std::endl;
		return 1;
	}
	return 0;
}

/* check and extract commands */
static bool	checkArgs(int ac, char *av[], t_Args *v) {

	if (ac < 3)
		return 1;

	int	curr = 0;
	v->hash = 0;
	v->configFile = "";
	for (int i = 1; i < ac; i++) {

		if (av[i][0] == '-') {
			if (extractFlag(ac, av, i, v))
				return (1);
		} else {
			switch (curr++)
			{
				case 0:
					v->port = stoi(av[i]);
					break ;
				case 1:
					v->password = std::string(av[i]);
					break ;
				default :
					std::cerr << "PARSING: error: unexpected argument: " << av[i] << std::endl;
					return 1;
			}
		}
	}
	if (curr != 2) {
		std::cerr << "PARSING: error: mandatory argument missing (must be <port> or <password>)" << std::endl;
		return 1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////

int	main(int ac, char *av[])
{
	t_Args	v;
	int		ret = 0;

	if (checkArgs(ac, av, &v)) {
		std::cerr << "Usage: ft_irc [--SHA256 or --MD5] [-h hash] [-c filename]" \
		" <port> <password>" << std::endl;
		return 2;
	}

	if (v.port < 1024 || v.port > 65535) {
		std::cerr << "PARSING: error: " << (v.port < 1024 ? "Reserved port" : "port doesn't exist") << std::endl;
		return 3;
	}

	std::signal(SIGINT, &endProg); /* initialize the signal handler to close the server */

	try {
		Server_ptr = new Server(v.port, (v.hash ? hash::md5(v.password) : hash::sha256(v.password)), v.hash);
		if (!v.configFile.empty())
			Server_ptr->getConfig(v.configFile);	/* get the config from JSON file */
		memset(&v, 0, sizeof(v));					/* Zeroize sensitive infos */
		Server_ptr->init();							/* init the socket and bind it to port */
		Server_ptr->run();							/* run the server and wait for client */
	}
	catch (std::exception &e) {
		std::cerr << "SERVER: error: " << e.what() << std::endl;
		ret = 127;
	}

	if (Server_ptr)
		delete Server_ptr;
	return ret;
}
