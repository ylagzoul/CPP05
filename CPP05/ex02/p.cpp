#include "ShrubberyCreationForm.hpp"
#include <fstream>
#include <iostream>

ShrubberyCreationForm::ShrubberyCreationForm(
    const std::string& target
)
    : AForm("ShrubberyCreationForm", 145, 137),
      _target(target)
{
}

ShrubberyCreationForm::ShrubberyCreationForm(
    const ShrubberyCreationForm& other
)
    : AForm(other),
      _target(other._target)
{
}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(
    const ShrubberyCreationForm& other
)
{
    if (this != &other)
    {
        AForm::operator=(other);
        _target = other._target;
    }

    return (*this);
}

ShrubberyCreationForm::~ShrubberyCreationForm()
{
}

void ShrubberyCreationForm::executeAction() const
{
    std::ofstream file((_target + "_shrubbery").c_str());

    if (!file)
    {
        std::cerr << "Error: cannot create shrubbery file." << std::endl;
        return;
    }

    file << "       /\\\n";
    file << "      /**\\\n";
    file << "     /****\\\n";
    file << "    /******\\\n";
    file << "   /********\\\n";
    file << "  /**********\\\n";
    file << " /************\\\n";
    file << "      ||||\n";
    file << "      ||||\n";

    file.close();
}