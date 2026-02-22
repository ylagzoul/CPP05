#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
    // ===== Test 1: Successful Sign =====
    std::cout << "=== Test 1: Successful Sign ===" << std::endl;
    try {
        Bureaucrat  boss("Youssef", 10);
        Form        form("Tax Form", 50, 25);
        std::cout << boss << std::endl;
        std::cout << "\nyousseff\n";
        std::cout << form << "\n" << std::endl;
        boss.signForm(form);
        std::cout << form << "\n" << std::endl;
    }
    catch (std::exception& e) { std::cout << "Exception: " << e.what() << std::endl; }

    // ===== Test 2: Failed Sign (grade too low) =====
    std::cout << "=== Test 2: Failed Sign ===" << std::endl;
    try {
        Bureaucrat  junior("Bob", 100);
        Form        form("Secret Form", 50, 25);
        std::cout << junior << std::endl;
        junior.signForm(form);
    }
    catch (std::exception& e) { std::cout << "Exception: " << e.what() << std::endl; }

    // ===== Test 3: Form grade out of bounds =====
    std::cout << "\n=== Test 3: Form grade 0 (too high) ===" << std::endl;
    try {
        Form badForm("Bad Form", 0, 25);
    }
    catch (std::exception& e) { std::cout << "Exception: " << e.what() << std::endl; }

    std::cout << "\n=== Test 4: Form grade 151 (too low) ===" << std::endl;
    try {
        Form badForm("Bad Form", 151, 25);
    }
    catch (std::exception& e) { std::cout << "Exception: " << e.what() << std::endl; }

    // ===== Test 4: Bureaucrat grade out of bounds =====
    std::cout << "\n=== Test 5: Bureaucrat grade -1 ===" << std::endl;
    try {
        Bureaucrat eve("Eve", -1);
    }
    catch (std::exception& e) { std::cout << "Exception: " << e.what() << std::endl; }

    // ===== Test 5: Exact grade match =====
    std::cout << "\n=== Test 6: Exact grade match ===" << std::endl;
    try {
        Bureaucrat  exact("Alice", 50);
        Form        form("Exact Form", 50, 25);
        std::cout << exact << std::endl;
        exact.signForm(form);
        std::cout << form << std::endl;
    }
    catch (std::exception& e) { std::cout << "Exception: " << e.what() << std::endl; }

    return (0);
}