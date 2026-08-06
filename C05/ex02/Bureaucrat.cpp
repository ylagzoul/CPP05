#include "Bureaucrat.hpp"
#include "AForm.hpp"
Bureaucrat::Bureaucrat(const std::string name,const int grade) : name(name), grade(grade)
{
    if (grade < 1)
        throw(GradeTooHighException());
    else if (grade > 150)
        throw (GradeTooLowException());
}

Bureaucrat::Bureaucrat(const Bureaucrat &other)
{
    grade = other.grade;
}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat &other)
{
    grade = other.grade;
    return (*this);
}

std::ostream& operator<<(std::ostream &os, Bureaucrat &Bureaucrat)
{
    os << Bureaucrat.getName() << ", bureaucrat grade " << Bureaucrat.getGrade();
    return (os);
}

std::string Bureaucrat::getName() const
{
    return (name);
}

int Bureaucrat::getGrade() const
{
    return(grade);
}

void Bureaucrat::increment()
{
    if (grade - 1 < 1)
        throw (GradeTooHighException());
    grade--;
}

void Bureaucrat::decrement()
{
    if (grade + 1 > 150)
        throw (GradeTooLowException());
    grade++;
}

void Bureaucrat::signForm(AForm &Aform)
{
    try
    {
        Aform.beSigned(*this);
        std::cout << getName() << " signed " << Aform.getName();
    }
    catch(const std::exception& e)
    {
        std::cout << name << " couldn’t sign " << Aform.getName() << " because " << e.what() << std::endl;
    }
    
}

void Bureaucrat::executeForm(const AForm& form)
{
    try
    {
        form.execute(*this);
        std::cout << "<bureaucrat> executed <form>" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

}

Bureaucrat::~Bureaucrat()
{
}