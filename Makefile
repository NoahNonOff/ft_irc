SRC =	srcs/main.cpp						\
		srcs/utils.cpp						\
		srcs/request.cpp					\
		srcs/commands.cpp					\
		srcs/client.cpp						\
		srcs/server.cpp						\
											\
		Hash/md5/srcs/Md5.cpp				\
		Hash/sha-256/srcs/Sha256.cpp		\
											\
		Json-parser/srcs/Parser.cpp			\
		Json-parser/srcs/Atype.cpp			\
		Json-parser/srcs/Object.cpp			\
		Json-parser/srcs/Array.cpp			\
		Json-parser/srcs/String.cpp			\
		Json-parser/srcs/Number.cpp			\
		Json-parser/srcs/Null.cpp			\
		Json-parser/srcs/Boolean.cpp		\

INCLUDE =	-I ./include					\
			-I ./Json-parser/include		\
			-I ./Hash/sha-256/include		\
			-I ./Hash/md5/include			\

OBJ = $(SRC:.cpp=.o)
CC = c++
RM = rm -f
DEBUG = 
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 $(DEBUG)

NAME = ft_irc

%.o : %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) -c $< -o ${<:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

cpu: ${OBJ}
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME) -pg
#	./${NAME} 4526 test						(run your program)
#	gprof ${NAME} gmon.out > cpu_usage.txt	(write the result in cpu_usage.txt)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re

.SILENT:
