#include "Form.hpp"


Form::Form(const int gradeToSign, const int gradeToExecute, const std::string name) : _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute), _name(name)
{
    if (_gradeToSign < 1 || _gradeToExecute < 1)
        throw (GradeTooHighException());
    else if (_gradeToSign > 150 || _gradeToExecute > 150)
        throw (GradeTooLowException());

    _signed = false;
}

Form::Form(const Form& other) : _gradeToExecute(other._gradeToExecute), _gradeToSign(other._gradeToSign), _name(other._name)
{
    _signed = other._signed;
}

Form& Form::operator=(const Form& other)
{
    _signed = other._signed;
    return (*this);
}

const std::string Form::getName()
{
    return (_name);
}

bool Form::getSigned()
{
    return (_signed);
}

const int Form::getGradeToSign()
{
    return (_gradeToSign);
}
const int Form::getGradeToExecute()
{
    return (_gradeToExecute);
}

void Form::beSigned(Bureaucrat &be)
{
    if (be.getGrade() > _gradeToSign)
        throw (GradeTooLowException());
    _signed = true;
}

std::ostream& operator<<(std::ostream& os, Form& form)
{
    os << form.getName() << ", Grade To Sign " << form.getGradeToSign()  << ", Grade To Execute "<< form.getGradeToExecute() << ", Signed " << form.getSigned();
}

Form::~Form()
{
}
