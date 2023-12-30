//
// Created by ykarroum on 12/26/23.
//

#ifndef CLOX_COMPILER_VALUE_H
#define CLOX_COMPILER_VALUE_H

#include <variant>

using Value = std::variant<bool, double, nullptr_t>;

void printValue(Value value);

#endif //CLOX_COMPILER_VALUE_H
