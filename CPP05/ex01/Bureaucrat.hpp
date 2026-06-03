#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <iostream>
#include <exception>
#include <string>

class Form;

class Bureaucrat {
    private:
        const std::string name;
        int grade;
    public:
        Bureaucrat(const std::string name, int grade);
        Bureaucrat(const Bureaucrat& other);
        Bureaucrat& operator=(const Bureaucrat& other);
        const std::string getName() const;
        int getGrade() const;
        void increment();
        void decrement();
        void signForm(Form& form);
        ~Bureaucrat();

        class GradeTooHighException : public std::exception {
            public:
                const char * what() const throw() {
                    return ("Bureaucrat: grade too high! (minimum is 1)");
                }
        };

        class GradeTooLowException : public std::exception {
            public:
                const char* what() const throw() {
                    return ("Bureaucrat: grade too low! (maximum is 150)");
                }
        };
};

std::ostream& operator<<(std::ostream& out, const Bureaucrat& b);

#endif