#include "Bureaucrat.hpp"
#include "AForm.hpp"


int main()
{
        try{
            Bureaucrat bob("youssef", 20);
            AForm Aform(100,59,"aya");
            bob.signForm(Aform);
        }
        catch(std::exception& e) {
            std::cout << e.what();
        }

        std::cout << "\n ----------------------------- \n";
        try{
            Bureaucrat bob("youssef", 130);
            AForm Aform(100,59,"aya");
            bob.signForm(Aform);
        }
        catch(std::exception& e) {
            std::cout << e.what();
        }

        std::cout << "\n ----------------------------- \n";

        try{
            Bureaucrat bob("youssef", 20);
            AForm form(0,59,"aya");
        }
        catch(std::exception& e) {
            std::cout << e.what();
        }

        std::cout << "\n ----------------------------- \n";

        try {
            Bureaucrat bob("youssef", 20);
            AForm Aform(65,159,"aya");
        }
        catch(std::exception& e) {
            std::cout << e.what();
        }
}