# Makefile
#
# Author: Noah BEAUFILS
# Date: 13-oct-2023

DIR =	srcs

SRC =	${DIR}/main.cpp				\
		${DIR}/utils.cpp			\
		${DIR}/client.cpp			\
		${DIR}/channel.cpp			\
		${DIR}/server.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -I include -g3 -std=c++98

NAME = ft_irc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

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
