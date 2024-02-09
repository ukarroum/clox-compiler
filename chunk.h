#pragma once

#include <cstdint>
#include <vector>
#include <cstdlib>

#include "common.h"
#include "value.h"

enum OpCode : uint8_t
{
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_RETURN,
    OP_NEGATE,
    OP_ADD,
    OP_SUBSTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT
};

class Chunk
{
public:
    void write(uint8_t byte, size_t line);
    void writeConstant(Value constant, int line);
    size_t addConstant(Value constant);
    [[nodiscard]] const std::vector<uint8_t>& getCode() const { return code; }
    [[nodiscard]] uint16_t getLine(int offset) const;
    [[nodiscard]] const std::vector<Value>& getConstants() const { return constants; }

private:
    std::vector<uint8_t> code;
    std::vector<std::pair<size_t, int>> lines;
    std::vector<Value> constants;
};
