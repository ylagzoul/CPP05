#ifndef  CONVERT_HHP
#define  CONVERT_HHP

#include <iostream>
#include <cstdlib>

class  ScalarConverter
{
    public:
        std::string Type_v;
        ScalarConverter();
        
        // ~ScalarConverter();
        int checkParameter(int ac, std::string type);

};


#endif