#include <iostream>

#include "value.h"

void printValue(Value value)
{
    if(std::holds_alternative<double>(value))
        std::cout << std::get<double>(value);
    else if(std::holds_alternative<nullptr_t>(value))
        std::cout << "NIL";
    else if(std::holds_alternative<bool>(value))
        std::cout << std::get<bool>(value);
    else if(std::holds_alternative<std::string>(value))
        std::cout << std::get<std::string>(value);
}