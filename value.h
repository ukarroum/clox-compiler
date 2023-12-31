//
// Created by ykarroum on 12/26/23.
//

#ifndef CLOX_COMPILER_VALUE_H
#define CLOX_COMPILER_VALUE_H

#include <variant>
#include <any>

// The reason why we're not just using any as Value, is that we want to be as efficient as possible for primitive types like numbers and booleans
using Value = std::variant<bool, double, nullptr_t, std::string>;

void printValue(Value value);

#endif //CLOX_COMPILER_VALUE_H
