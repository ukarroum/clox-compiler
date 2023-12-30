//
// Created by ykarroum on 12/27/23.
//

#ifndef CLOX_COMPILER_VM_H
#define CLOX_COMPILER_VM_H

#include <stack>
#include <functional>

#include "chunk.h"

enum class InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VM
{
public:
    InterpretResult interpret(const std::string& code);
    InterpretResult run();
private:
    uint8_t readByte() { return *m_ip++; }
    Value readConstant() { return m_chunk.getConstants()[readByte()]; }
    template <typename BinaryOp> void binaryOp(BinaryOp op);

    Chunk m_chunk;
    std::vector<uint8_t>::const_iterator m_ip;
    std::stack<Value> m_stack;
};



#endif //CLOX_COMPILER_VM_H
