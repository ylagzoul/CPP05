#ifndef BUREACRAT_HPP
# define BUREACRAT_HPP

#include <iostream>


class AForm;

class Bureaucrat
{
    private:
        const std::string   name;
        int                 grade;
    public:
        Bureaucrat(const std::string name, int grade);
        Bureaucrat(const Bureaucrat &other);
        Bureaucrat &operator=(const Bureaucrat &other);
        std::string getName() const;
        int getGrade() const;
        void increment();
        void decrement();
        void signForm(AForm &Aform);
        void executeForm(const AForm& form);
        ~Bureaucrat();
        class GradeTooHighException : public std::exception
        {
            public:
                const char* what() const throw()
                {
                    return ("grade too high");
                }
        };
        class GradeTooLowException : public std::exception
        {
            public:
                const char* what() const throw()
                {
                    return ("grade too low");
                }
        };
        
};

std::ostream& operator<<(std::ostream &os, Bureaucrat& Bureaucrat);

#endif

