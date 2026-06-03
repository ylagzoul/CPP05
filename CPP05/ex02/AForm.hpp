#ifndef AFORM_HPP
#define AFORM_HPP

#include <iostream>
#include <exception>
#include <string>

class Bureaucrat;

class AForm {
    private:
        const std::string   _name;
        bool                _signed;
        const int           _gradeToSign;
        const int           _gradeToExecute;
    public:
        AForm(const std::string name, const int gradeToSign, const int gradeToExecute);
        AForm(const AForm& other);
        AForm& operator=(const AForm& other);
        ~AForm();

        const std::string   getName()           const;
        bool                isSigned()          const;
        int                 getGradeToSign()    const;
        int                 getGradeToExecute() const;

        void beSigned(const Bureaucrat& b) = 0;

        class GradeTooHighException : public std::exception {
            public:
                const char* what() const throw() {
                    return ("AForm: grade too high! (minimum is 1)");
                }
        };

        class GradeTooLowException : public std::exception {
            public:
                const char* what() const throw() {
                    return ("AForm: grade too low! (maximum is 150)");
                }
        };
};

std::ostream& operator<<(std::ostream& os, const AForm& Aform);

#endif