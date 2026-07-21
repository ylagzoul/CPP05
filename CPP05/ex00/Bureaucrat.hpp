#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <iostream>
#include <exception>

class GradeTooHighException : public std::exception {
    public:
        const char * what() const throw() {
            return ("grade too highest");
        }
};

class GradeTooLowException : public std::exception {
    public:
        const char* what() const throw() {
            return ("greade too lowest");
        }
};


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
        ~Bureaucrat();
};

std::ostream& operator<<(std::ostream& out,const Bureaucrat& Bureaucrat);

#endif