#ifndef SHRUBBERYCREATIONFORM_HPP
#define SHRUBBERYCREATIONFORM_HPP
#include "AForm.hpp"

class ShrubberyCreationForm : public AForm
{
    private:
        std::string name;
    public:
        ShrubberyCreationForm(std::string name);
        
        ~ShrubberyCreationForm();
};

#endif