SRC =	main.cpp					\
		sha-256/srcs/Sha256.cpp		\
		md5/srcs/Md5.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
RM = rm -f
DEBUG = 
INCLUDE = -I ./sha-256/include -I ./md5/include
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 $(DEBUG)

NAME = hash

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
