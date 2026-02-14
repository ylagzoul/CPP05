#include "Bureaucrat.hpp"

int main() {
    
    
    std::cout << "\n=== Test 5: Decrement Grade ===" << std::endl;
    try {
        Bureaucrat eve("Eve", 149);
        // std::cout << eve << std::endl;
        eve.decrement();
        // std::cout << "After decrement: " << eve << std::endl;
        eve.increment(); // Should throw exception
        // std::cout << "After second decrement: " << eve << std::endl;
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
    

    return 0;
}