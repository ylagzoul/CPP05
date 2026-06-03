#ifndef SHRUBBERYCREATIONFORM_HPP
#define SHRUBBERYCREATIONFORM_HPP

#include "Bureaucrat.hpp"

class ShrubberyCreationForm : public AForm
{
private:
public:
    void beSigned(const Bureaucrat &b);
};

#endif