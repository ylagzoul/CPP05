NAME = Serializer
CC = c++
FLAG = -Wextra -Wall -Werror -std=c++98
SRC = Serializer.cpp
OBJ = $(SRC:%.cpp=%.o)
HADER = Serializer.hpp

all:$(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAG) $(OBJ) -o $(NAME)

%.o:%.cpp $(HADER)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re:fclean all
