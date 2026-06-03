#ifndef  CONVERT_HHP
#define  CONVERT_HHP

#include <iostream>

class  ScalarConverter
{
    public:
        std::string Type_v;
        ScalarConverter();
        
        // ~ScalarConverter();
        int checkParameter(int ac, std::string type);

};


#endif