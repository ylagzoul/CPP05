#include "AForm.hpp"


AForm::AForm(const int gradeToSign, const int gradeToExecute, const std::string name) : _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute), _name(name)
{
    if (_gradeToSign < 1 || _gradeToExecute < 1)
        throw (GradeTooHighException());
    else if (_gradeToSign > 150 || _gradeToExecute > 150)
        throw (GradeTooLowException());

    _signed = false;
}

AForm::AForm(const AForm& other) : _gradeToExecute(other._gradeToExecute), _gradeToSign(other._gradeToSign), _name(other._name)
{
    _signed = other._signed;
}

AForm& AForm::operator=(const AForm& other)
{
    _signed = other._signed;
    return (*this);
}

const std::string AForm::getName()
{
    return (_name);
}

bool AForm::getSigned()
{
    return (_signed);
}

const int AForm::getGradeToSign()
{
    return (_gradeToSign);
}
const int AForm::getGradeToExecute()
{
    return (_gradeToExecute);
}

void AForm::beSigned(Bureaucrat &be)
{
    if (be.getGrade() > _gradeToSign)
        throw (GradeTooLowException());
    _signed = true;
}

void AForm::execute(const Bureaucrat& executor)
{
    // if (__signed)
    ;

}

std::ostream& operator<<(std::ostream& os, AForm& Aform)
{
    os << Aform.getName() << ", Grade To Sign " << Aform.getGradeToSign()  << ", Grade To Execute "<< Aform.getGradeToExecute() << ", Signed " << Aform.getSigned();
    return (os);
}

AForm::~AForm()
{
}
