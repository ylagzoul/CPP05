NAME = Bureaucrat
CC = c++
FLAG = -Wextra -wall -Werror -std=c++98
SRC = main.cpp Bureaucrat.cpp
OBJ = $(SRC:%.cpp=%.o)
HADER = Bureaucrat.hpp

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
