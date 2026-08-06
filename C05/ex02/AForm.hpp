#ifndef AFORM_HPP
#define AFORM_HPP

#include <iostream>
#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"

class Bureaucrat;

class AForm : ShrubberyCreationForm
{
    private:
        const std::string   _name;
        bool                _signed;
        const int           _gradeToSign;
        const int           _gradeToExecute;
    public:
        AForm(const int gradeToSign, const int gradeToExecute, const std::string name);
        AForm(const AForm& other);
        AForm& operator=(const AForm& other);
        const std::string getName();
        bool getSigned();
        const int getGradeToSign();
        const int getGradeToExecute();
        void beSigned(Bureaucrat &be);
        void execute(const Bureaucrat& executor);
        ~AForm();
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

std::ostream& operator<<(std::ostream& os, AForm& Aform);

#endif