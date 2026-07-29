#ifndef BUREACRAT_HPP
# define BUREACRAT_HPP

#include <iostream>


class Form;

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
        void signForm(Form &form);
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

