#include "Bureaucrat.hpp"
#include "Form.hpp"


int main()
{
        try{
            Bureaucrat bob("youssef", 20);
            Form form(100,59,"aya");
            bob.signForm(form);
        }
        catch(std::exception& e) {
            std::cout << e.what();
        }

        std::cout << "\n ----------------------------- \n";
        try{
            Bureaucrat bob("youssef", 130);
            Form form(100,59,"aya");
            bob.signForm(form);
        }
        catch(std::exception& e) {
            std::cout << e.what();
        }

        std::cout << "\n ----------------------------- \n";

        try{
            Bureaucrat bob("youssef", 20);
            Form form(0,59,"aya");
        }
        catch(std::exception& e) {
            std::cout << e.what();
        }

        std::cout << "\n ----------------------------- \n";

        try {
            Bureaucrat bob("youssef", 20);
            Form form(65,159,"aya");
        }
        catch(std::exception& e) {
            std::cout << e.what();
        }
}