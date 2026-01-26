#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <iostream>

class Bureaucrat
{
    private:
        const std::string name;
    public:
        const std::string getName();
        void getGrade();
        void GradeTooHighException();
        void GradeTooLowException();
};




#endif