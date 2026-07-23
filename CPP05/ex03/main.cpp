#include "Intern.hpp"
#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include <iostream>

int main()
{
    Intern intern;
    Bureaucrat boss("Boss", 1);

    AForm *form1 = NULL;
    AForm *form2 = NULL;
    AForm *form3 = NULL;
    AForm *form4 = NULL;

    // ==========================================
    // TEST 1: Shrubbery Creation Form
    // ==========================================
    std::cout << "\n========== TEST 1 ==========" << std::endl;

    form1 = intern.makeForm("Shrubbery Creation", "home");

    if (form1)
    {
        std::cout << "Form created: " << form1->getName() << std::endl;

        boss.signForm(*form1);
        boss.executeForm(*form1);

        delete form1;
        form1 = NULL;
    }

    // ==========================================
    // TEST 2: Robotomy Request Form
    // ==========================================
    std::cout << "\n========== TEST 2 ==========" << std::endl;

    form2 = intern.makeForm("Robotomy Request", "Bender");

    if (form2)
    {
        std::cout << "Form created: " << form2->getName() << std::endl;

        boss.signForm(*form2);
        boss.executeForm(*form2);

        delete form2;
        form2 = NULL;
    }

    // ==========================================
    // TEST 3: Presidential Pardon Form
    // ==========================================
    std::cout << "\n========== TEST 3 ==========" << std::endl;

    form3 = intern.makeForm("Presidential Pardon", "Arthur");

    if (form3)
    {
        std::cout << "Form created: " << form3->getName() << std::endl;

        boss.signForm(*form3);
        boss.executeForm(*form3);

        delete form3;
        form3 = NULL;
    }

    // ==========================================
    // TEST 4: Invalid Form
    // ==========================================
    std::cout << "\n========== TEST 4 ==========" << std::endl;

    form4 = intern.makeForm("Invalid Form", "Nobody");

    if (form4 == NULL)
    {
        std::cout << "Invalid form: correctly returned NULL" << std::endl;
    }

    // ==========================================
    // END
    // ==========================================

    return 0;
}