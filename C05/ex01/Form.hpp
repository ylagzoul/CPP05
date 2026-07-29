#ifndef FORM_HPP
#define FORM_HPP

#include <iostream>
#include "Bureaucrat.hpp"

class Bureaucrat;

class Form
{
    private:
        const std::string   _name;
        bool                _signed;
        const int           _gradeToSign;
        const int           _gradeToExecute;
    public:
        Form(const int gradeToSign, const int gradeToExecute, const std::string name);
        Form(const Form& other);
        Form& operator=(const Form& other);
        const std::string getName();
        bool getSigned();
        const int getGradeToSign();
        const int getGradeToExecute();
        void beSigned(Bureaucrat &be);

        ~Form();
        class GradeTooHighException : public std::exception
        {
            public:
            const char* what() const throw()
            {
                return ("grade too high exeption");
            }
        };

        class GradeTooLowException : public std::exception
        {
            public:
            const char* what() const throw()
            {
                return ("grade too low exception");
            }
        };
};

std::ostream& operator<<(std::ostream& os, Form& form);

#endif