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