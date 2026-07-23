#ifndef INTERN_HPP
#define INTERN_HPP
#include <iostream>
#include "AForm.hpp"

class Intern
{
    public:
        // Intern();
        AForm *makeForm(std::string nameToForm, std::string targetToForm);
        AForm *Shrubbery(std::string target);
        AForm *Robotomy(std::string target);
        AForm *Presidential(std::string target);
        // ~Intern();
};

#endif