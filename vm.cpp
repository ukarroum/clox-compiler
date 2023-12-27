//
// Created by ykarroum on 12/27/23.
//

#include <iostream>
#include <functional>

#include "vm.h"
#include "debug.h"

InterpretResult VM::interpret(Chunk *chunk)
{
    m_chunk = chunk;
    m_ip = m_chunk->getCode().cbegin();

    return run();
}

InterpretResult VM::run()
{
    while(true)
    {
#ifdef DEBUG_TRACE_EXECUTION
        disInstr(*m_chunk, m_ip - m_chunk->getCode().cbegin());
#endif

        uint8_t instr;
        switch(instr = readByte())
        {
            case OpCode::OP_RETURN:
            {
                auto val = m_stack.top();
                m_stack.pop();

                printValue(val);
                std::cout << std::endl;

                return InterpretResult::INTERPRET_OK;
            }
            case OpCode::OP_CONSTANT:
            {
                Value constant = readConstant();
                m_stack.push(constant);
                break;
            }
            case OpCode::OP_NEGATE:
            {
                auto top = m_stack.top();
                m_stack.pop();
                m_stack.push(-top);
                break;
            }
            case OpCode::OP_ADD:
                binaryOp(std::plus<>());
                break;
            case OpCode::OP_SUBSTRACT:
                binaryOp(std::minus<>());
                break;
            case OpCode::OP_MULTIPLY:
                binaryOp(std::multiplies<>());
                break;
            case OpCode::OP_DIVIDE:
                binaryOp(std::divides<>());
                break;
        }
    }
}

template <typename BinaryOp> void VM::binaryOp(BinaryOp op) {
    Value b = m_stack.top();
    m_stack.pop();
    Value a = m_stack.top();
    m_stack.pop();

    m_stack.push(op(a, b));
}
