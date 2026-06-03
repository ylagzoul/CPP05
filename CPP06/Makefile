NAME = convert
CC = c++
FLAGS = -Werror -Wextra -Wall -std=c++98
SRC = convert.cpp
OBJ = $(SRC:%.cpp=%.o)
HADER = convert.hpp

all: $(NAME)

$(NAME):$(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o:%.cpp $(HADER)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean:clean
	rm -rf $(NAME)

re:fclean all