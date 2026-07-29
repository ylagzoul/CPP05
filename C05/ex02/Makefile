NAME = form
CC = c++ 
GLAG = -Wextra -Wall -Werror -std=c++98
SRC = main.cpp Bureaucrat.cpp AForm.cpp
OBJ = $(SRC:%.cpp=%.o)
HADER = Bureaucrat.hpp AForm.hpp

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