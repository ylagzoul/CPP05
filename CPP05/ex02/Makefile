NAME    = form
CC      = c++
FLAG    = -Wextra -Wall -Werror -std=c++98

SRC = main.cpp \
      Bureaucrat.cpp \
      AForm.cpp \
      ShrubberyCreationForm.cpp \
      RobotomyRequestForm.cpp \
      PresidentialPardonForm.cpp

OBJ     = $(SRC:%.cpp=%.o)
HEADERS = Bureaucrat.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAG) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CC) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all