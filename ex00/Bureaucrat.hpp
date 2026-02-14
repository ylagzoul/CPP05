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
        const std::string getName();
        Bureaucrat(Bureaucrat& other);
        void getGrade();
        void increment();
        void decrement();
        ~Bureaucrat();
};




#endif