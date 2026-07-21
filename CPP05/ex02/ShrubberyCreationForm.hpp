#ifndef SHRUBBERYCREATIONFORM_HPP
#define SHRUBBERYCREATIONFORM_HPP

#include "Bureaucrat.hpp"

class ShrubberyCreationForm : public AForm
{
    private:
        int _shrubbery;
    public:
        ShrubberyCreationForm();
        void beSigned(const Bureaucrat &b);
        void beExcec(const Bureaucrat& b);
};


#endif