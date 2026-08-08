NAME = iter
CC = c++
FLAG = -Wall -Wextra -Werror -std=c++98
SRC = main.cpp
OBJ = $(SRC:%.cpp=%.o)
HADER = iter.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAG) $(OBJ) -o $(NAME)

%.o:%.cpp $(NAME)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re:fclean all