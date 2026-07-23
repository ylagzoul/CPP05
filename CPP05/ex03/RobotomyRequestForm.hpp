#ifndef ROBOTOMYREQUESTFORM_HPP
#define ROBOTOMYREQUESTFORM_HPP

#include "AForm.hpp"

class RobotomyRequestForm : public AForm
{
private:
    std::string _target;

public:
    RobotomyRequestForm(const std::string& target);
    RobotomyRequestForm(const RobotomyRequestForm& other);

    RobotomyRequestForm& operator=(
        const RobotomyRequestForm& other
    );

    ~RobotomyRequestForm();

    void executeAction() const;
};

#endif