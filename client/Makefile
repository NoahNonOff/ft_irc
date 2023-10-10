# Makefile
#
# Author: Noah BEAUFILS
# Date: 10-oct-2023

SRC =	clt_main.cpp		\
		clt_visual.cpp		\
		clt.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
RM = rm -f
CPPFLAGS = -Wall -Wextra -Werror -I . -std=c++98

NAME = duck

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
