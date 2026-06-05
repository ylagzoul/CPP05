#include <iostream>
#include <sstream>

using namespace std;

int main(int ac, char **av)
{

    stringstream ss(av[1]);
    float f;

    ss >> f;
    string e;
    ss >> e;

    cout << f <<"\n";
    cout << "fail: " << ss.fail() <<"\n";
    cout << "eof: " << ss.eof() <<"\n";
    cout << "e: " << e <<"\n";

}




/* Errors

./convert -127.999999999
char: Non displayable
int: -127
float: -128f
double: -128



./convert -28615428752754725471252874532127462537645712547234324767324.32
char: impossible
int: -2147483648
float: -inff
double: -2.86154e+58





./convert -32547624428645721476                                          
char: Non displayable
int: 0
float: 0.0f
double: 0.0




./convert 32547624428645721476 
char: Non displayable
int: -1
float: -1.0f
double: -1.0
*/



