#include "Serializer.hpp"




uintptr_t Serializer::serialize(Data* ptr)
{
    uintptr_t num = reinterpret_cast<uintptr_t>(ptr);
    return (num);
}


Data* Serializer::deserialize(uintptr_t raw)
{
    Data *ptr = reinterpret_cast<Data *>(raw);
    return(ptr);
}



int main()
{
    Data ptr;
    uintptr_t num = Serializer::serialize(&ptr);
    std::cout << &ptr << std::endl;
    std::cout << num << std::endl;
    std::cout << Serializer::deserialize(num) << std::endl;
    return (0);
}