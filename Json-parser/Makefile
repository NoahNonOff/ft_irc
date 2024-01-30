JSON =	srcs

SRC =	main.cpp						\
		${JSON}/Parser.cpp				\
		${JSON}/Atype.cpp				\
		${JSON}/Object.cpp				\
		${JSON}/Array.cpp				\
		${JSON}/String.cpp				\
		${JSON}/Number.cpp				\
		${JSON}/Null.cpp				\
		${JSON}/Boolean.cpp				\

OBJ = $(SRC:.cpp=.o)
CC = c++
RM = rm -f
DEBUG = 
INCLUDE = -I ./include
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 $(DEBUG)

NAME = test

%.o : %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) -c $< -o ${<:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(INCLUDE) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re

.SILENT:
