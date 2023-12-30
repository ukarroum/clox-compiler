//
// Created by ykarroum on 12/27/23.
//

#include <iostream>

#include "compiler.h"
#include "Scanner.h"

bool Compiler::compile(std::string& code, Chunk& chunk)
{
    m_scanner = Scanner {code};

    m_code = code;
    m_compilingChunk = chunk;

    advance();
    expression();
    consume(TokenType::TOKEN_EOF, "Expect end of expression");

    endComiler();
    return !m_parser.hadError;
}

void Compiler::advance()
{
    m_parser.previous = m_parser.current;

    while(true)
    {
        m_parser.current = m_scanner.scanToken();

        if(m_parser.current.type == TokenType::ERROR) {
            errorAtCurrent(m_parser.current.errorMsg);
            break;
        }
    }
}

void Compiler::errorAtCurrent(const std::string& error)
{
    errorAt(m_parser.current, error);
}

void Compiler::error(const std::string& error)
{
    errorAt(m_parser.previous, error);
}

void Compiler::errorAt(const Token &token, const std::string &error)
{
    if(m_parser.panicMode) return;

    m_parser.panicMode = true;

    std::cerr << "[line " << token.line << "] Error " << error << std::endl;

    m_parser.hadError = true;
}

void Compiler::consume(TokenType type, const std::string &error)
{
    if(m_parser.current.type== type)
    {
        advance();
        return;
    }

    errorAtCurrent(error);
}

void Compiler::emitByte(uint8_t byte)
{
    currentChunk().write(byte, m_parser.previous.line);
}

Chunk& Compiler::currentChunk()
{
    return m_compilingChunk;
}

void Compiler::endCompiler()
{
    emitReturn();
}

void Compiler::emitReturn()
{
    emitByte(OpCode::OP_RETURN);
}

void Compiler::emitBytes(uint8_t byte1, uint8_t byte2)
{
    emitByte(byte1);
    emitByte(byte2);
}

void Compiler::expression()
{
    parsePrecedence(Precedence::ASSIGNEMENT);
}

void Compiler::number()
{
    Value value = std::stod(m_code.substr(m_parser.previous.start, m_parser.previous.length));
    emitConstant(value);
}

void Compiler::emitConstant(Value value)
{
    emitBytes(OP_CONSTANT, makeConstant(value));
}

uint8_t Compiler::makeConstant(Value value)
{
    auto constant = currentChunk().addConstant(value);

    if(constant > UINT8_MAX)
    {
        error("Too many constants in one chunk.");
        return 0;
    }

    return static_cast<uint8_t>(constant);
}

void Compiler::grouping()
{
    expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
}

void Compiler::unary()
{
    TokenType op = m_parser.previous.type;

    parsePrecedence(Precedence::UNARY);

    switch(op)
    {
        case TokenType::BANG:
            emitByte(OpCode::OP_NOT);
            break;
        case TokenType::MINUS:
            emitByte(OpCode::OP_NEGATE);
            break;
        default:
            return;
    }
}

void Compiler::parsePrecedence(Precedence precedence)
{
    advance();
    auto prefixRule = getRule(m_parser.previous.type).prefix;

    if(prefixRule == nullptr)
    {
        error("Expect expression.");
        return;
    }

    prefixRule();

    while(precedence <= getRule(m_parser.current.type).precedence)
    {
        advance();
        auto infixRule = getRule(m_parser.previous.type).infix;
        infixRule();
    }
}

void Compiler::binary()
{
    TokenType op = m_parser.previous.type;
    ParseRule rule = getRule(op);
    parsePrecedence(static_cast<Precedence>(static_cast<int>(rule.precedence) + 1));

    switch(op)
    {
        case TokenType::BANG_EQUAL:
            emitBytes(OP_EQUAL, OP_NOT);
            break;
        case TokenType::EQUAL_EQUAL:
            emitByte(OP_EQUAL);
            break;
        case TokenType::GREATER:
            emitByte(OP_GREATER);
            break;
        case TokenType::GREATER_EQUAL:
            emitBytes(OP_LESS, OP_NOT);
            break;
        case TokenType::LESS:
            emitByte(OP_LESS);
            break;
        case TokenType::LESS_EQUAL:
            emitBytes(OP_GREATER, OP_NOT);
            break;
        case TokenType::PLUS:
            emitByte(OP_ADD);
            break;
        case TokenType::MINUS:
            emitByte(OP_SUBSTRACT);
            break;
        case TokenType::STAR:
            emitByte(OP_MULTIPLY);
            break;
        case TokenType::SLASH:
            emitByte(OP_DIVIDE);
            break;
        default:
            return;
    }
}

ParseRule Compiler::getRule(TokenType type)
{
    return m_rules.at(type);
}

void Compiler::literal()
{
    switch(m_parser.previous.type)
    {
        case TokenType::FALSE:
            emitByte(OpCode::OP_FALSE);
            break;
        case TokenType::TRUE:
            emitByte(OpCode::OP_TRUE);
            break;
        case TokenType::NIL:
            emitByte(OpCode::OP_NIL);
            break;
        default:
            return;
    }
}

