# Makefile
#
# Author: Noah BEAUFILS
# Date: 8-jan-2024

DIR =	srcs

SRC =	${DIR}/main.cpp				\
		${DIR}/utils.cpp			\
		${DIR}/client.cpp			\
		${DIR}/server.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
RM = rm -f
DEBUG = 
INCLUDE = -I ./include
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 $(DEBUG)

NAME = ft_irc

%.o : %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) -c $<

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

# .SILENT:
