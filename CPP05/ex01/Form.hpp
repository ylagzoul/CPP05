#ifndef FORM_HPP
#define FORM_HPP

#include <iostream>
#include <exception>
#include <string>

class Bureaucrat;

class GradeTooHighException : public std::exception {
    public:
        const char* what() const throw() {
            return ("Form: grade too high! (minimum is 1)");
        }
};

class GradeTooLowException : public std::exception {
    public:
        const char* what() const throw() {
            return ("Form: grade too low! (maximum is 150)");
        }
};

class Form {
    private:
        const std::string   _name;
        bool                _signed;
        const int           _gradeToSign;
        const int           _gradeToExecute;
    public:
        Form(const std::string name, const int gradeToSign, const int gradeToExecute);
        Form(const Form& other);
        Form& operator=(const Form& other);
        ~Form();

        const std::string   getName()           const;
        bool                isSigned()          const;
        int                 getGradeToSign()    const;
        int                 getGradeToExecute() const;

        void beSigned(const Bureaucrat& b);

};

std::ostream& operator<<(std::ostream& os, const Form& form);

#endif