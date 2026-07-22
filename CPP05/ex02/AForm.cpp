#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm(const std::string name, const int gradeToSign, const int gradeToExecute) : _name(name), _signed(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
    if (gradeToSign < 1 || gradeToExecute < 1)
        throw GradeTooHighException();

    if (gradeToSign > 150 || gradeToExecute > 150)
        throw GradeTooLowException();
}

AForm::AForm(const AForm& other) : _name(other._name), _signed(other._signed), _gradeToSign(other._gradeToSign), _gradeToExecute(other._gradeToExecute)
{
}

AForm& AForm::operator=(const AForm& other)
{
    if (this != &other)
        _signed = other._signed;

    return (*this);
}

AForm::~AForm()
{
}

const std::string AForm::getName() const
{
    return (_name);
}

bool AForm::isSigned() const
{
    return (_signed);
}

int AForm::getGradeToSign() const
{
    return (_gradeToSign);
}

int AForm::getGradeToExecute() const
{
    return (_gradeToExecute);
}

void AForm::beSigned(const Bureaucrat& b)
{
    if (b.getGrade() > _gradeToSign)
        throw GradeTooLowException();
    _signed = true;
}

void AForm::execute(Bureaucrat const& executor) const
{
    if (!_signed)
        throw FormNotSignedException();

    if (executor.getGrade() > _gradeToExecute)
        throw GradeTooLowToExecuteException();

    executeAction();
}

std::ostream& operator<<(std::ostream& os, const AForm& form)
{
    os << "AForm name        : " << form.getName()
       << "\nSigned           : "
       << (form.isSigned() ? "Yes" : "No")
       << "\nGrade to Sign    : "
       << form.getGradeToSign()
       << "\nGrade to Execute : "
       << form.getGradeToExecute();

    return (os);
}