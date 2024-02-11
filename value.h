#pragma once

#include <variant>
#include <string>

// The reason why we're not just using any as Value, is that we want to be as efficient as possible for primitive types like numbers and booleans
using Value = std::variant<bool, double, nullptr_t, std::string>;

void printValue(Value value);
