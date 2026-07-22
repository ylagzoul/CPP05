#include "RobotomyRequestForm.hpp"
#include <iostream>
#include <cstdlib>

RobotomyRequestForm::RobotomyRequestForm(const std::string& target): AForm("RobotomyRequestForm", 72, 45), _target(target)
{
}

RobotomyRequestForm::RobotomyRequestForm(
    const RobotomyRequestForm& other
)
    : AForm(other),
      _target(other._target)
{
}

RobotomyRequestForm& RobotomyRequestForm::operator=(
    const RobotomyRequestForm& other
)
{
    if (this != &other)
    {
        AForm::operator=(other);
        _target = other._target;
    }

    return (*this);
}

RobotomyRequestForm::~RobotomyRequestForm()
{
}

void RobotomyRequestForm::executeAction() const
{
    std::cout
        << "Brrrrrrrrrrrrrr..."
        << std::endl;

    std::cout
        << "Drilling noises..."
        << std::endl;

    if (std::rand() % 2 == 0)
    {
        std::cout
            << _target
            << " has been robotomized successfully."
            << std::endl;
    }
    else
    {
        std::cout
            << "Robotomy failed."
            << std::endl;
    }
}