#include "Intern.hpp"
#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"


AForm *Intern::Shrubbery(std::string target)
{
    return(new ShrubberyCreationForm(target));
}

AForm *Intern::Robotomy(std::string target)
{
    return (new RobotomyRequestForm(target));
}

AForm *Intern::Presidential(std::string target)
{
    return (new PresidentialPardonForm(target));
}

AForm *Intern::makeForm(std::string nameToForm, std::string targetToForm)
{
    int i = 0;
    const std::string av[] = {"Shrubbery Creation","Robotomy Request","Presidential Pardon"};
    AForm * (Intern::*print[])(std::string) = {&Intern::Shrubbery,&Intern::Robotomy,&Intern::Presidential};

    while (i < 3)
    {
        if (nameToForm == av[i])
        {
            return ((this->*print[i])(targetToForm));
        }
        i++;
    }
    return (NULL);
}