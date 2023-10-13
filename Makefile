# Makefile
#
# Author: Noah BEAUFILS
# Date: 13-oct-2023

DIR =	srcs

SRC =	${DIR}/main.cpp				\
		${DIR}/utils.cpp			\
		${DIR}/client.cpp			\
		${DIR}/channel.cpp			\
		${DIR}/server.cpp			\
		${DIR}/command.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -I include -g3 -std=c++98

NAME = ft_irc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

#cpu_usage: ${OBJ}
#	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME) -pg
#	./${NAME} 4526 test
#	gprof ${NAME} gmon.out > cpu_usage.txt
#	rm gmon.out

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re

.SILENT:
