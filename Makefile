# Makefile
#
# Author: Noah BEAUFILS
# Date: 3-oct-2023

SRC =	main.cpp			\
		lib.cpp				\
		client.cpp			\
		handle_client.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -lpthread -std=c++98

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
