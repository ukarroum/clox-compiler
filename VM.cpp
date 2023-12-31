//
// Created by ykarroum on 12/27/23.
//

#include <iostream>
#include <functional>

#include "VM.h"
#include "debug.h"
#include "compiler.h"

InterpretResult VM::interpret(const std::string& code)
{
    m_chunk = Chunk();

    Compiler compiler;

    if(!compiler.compile(code, chunk))
        return InterpretResult::INTERPRET_COMPILE_ERROR;

    m_ip = m_chunk.getCode().cbegin();

    InterpretResult result = run();

    return result;
}

InterpretResult VM::run()
{
    while(true)
    {
#ifdef DEBUG_TRACE_EXECUTION
        disInstr(m_chunk, m_ip - m_chunk.getCode().cbegin());
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
            case OpCode::OP_NIL:
                m_stack.emplace(0);
                break;
            case OpCode::OP_TRUE:
                m_stack.emplace(true);
                break;
            case OpCode::OP_FALSE:
                m_stack.emplace(false);
                break;
            case OpCode::OP_EQUAL:
            {
                Value b = m_stack.top();
                m_stack.pop();
                Value a = m_stack.top();
                m_stack.pop();

                m_stack.emplace(a == b);
            }
            case OpCode::OP_GREATER:
                binaryOp(std::greater<>());
                break;
            case OpCode::OP_LESS:
                binaryOp(std::less<>());
                break;
            case OpCode::OP_NEGATE:
            {
                auto top = m_stack.top();
                m_stack.pop();
                if(!std::holds_alternative<double>(top))
                {
                    runtimeError("Operand must be a number");
                    return InterpretResult::INTERPRET_RUNTIME_ERROR;
                }
                m_stack.emplace(-std::get<double>(top));
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
            case OpCode::OP_NOT:
                auto top = m_stack.top();
                m_stack.pop();

                if(std::holds_alternative<bool>(top))
                    m_stack.emplace(!std::get<bool>(top));
                else if(std::holds_alternative<nullptr_t>(top))
                    m_stack.emplace(true);
                else
                    m_stack.emplace(false);
                break;
        }
    }
}

void VM::runtimeError(const std::string &msg)
{
    std::cerr << msg << std::endl;
}

template<>
void VM::binaryOp<std::plus<>>(std::plus<> op) {
    Value b = m_stack.top();
    m_stack.pop();
    Value a = m_stack.top();
    m_stack.pop();

    if(std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
        m_stack.emplace(std::get<double>(a) + std::get<double>(b));
    else if(std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(a))
        m_stack.emplace(std::get<std::string>(a) + std::get<std::string>(b));
    else
        runtimeError("Operands must be numbers");
}

template <typename BinaryOp> void VM::binaryOp(BinaryOp op) {
    Value b = m_stack.top();
    m_stack.pop();
    Value a = m_stack.top();
    m_stack.pop();

    if(!std::holds_alternative<double>(a) || !std::holds_alternative<double>(b))
    {
        runtimeError("Operands must be numbers");
        exit(1);
        //return InterpretResult::INTERPRET_RUNTIME_ERROR;
    }

    m_stack.push(op(std::get<double>(a), std::get<double>(b)));
}

