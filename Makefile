# Makefile
#
# Author: Noah BEAUFILS
# Date: 8-oct-2023

DIR =	srcs

SRC =	${DIR}/main.cpp				\
		${DIR}/utils.cpp			\
		${DIR}/client.cpp			\
		${DIR}/server.cpp			\
		${DIR}/command.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -I include -lpthread -std=c++98

NAME = ft_irc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re

.SILENT:
