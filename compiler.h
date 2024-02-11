#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include "Scanner.h"
#include "chunk.h"

struct Parser {
    Token current;
    Token previous;
    bool hadError = false;
    bool panicMode = false;
};

enum class Precedence {
    NONE,
    ASSIGNEMENT,
    OR,
    AND,
    EQUALITY,
    COMPARISON,
    TERM,
    FACTOR,
    UNARY,
    CALL,
    PRIMARY
};

struct ParseRule {
    std::function<void(void)> prefix;
    std::function<void(void)> infix;
    Precedence precedence;
};


class Compiler {

public:
    bool compile(std::string& code, Chunk& chunk);
    void advance();
    void errorAtCurrent(const std::string& error);
    void error(const std::string& error);
    void errorAt(const Token &token, const std::string& error);
    void consume(TokenType type, const std::string& error);
    void emitByte(uint8_t byte);
    void emitBytes(uint8_t byte1, uint8_t byte2);
    void emitReturn();
    void emitConstant(Value value);
    Chunk& currentChunk();
    void endCompiler();
    void expression();
    void number();
    void grouping();
    void unary();
    void binary();
    void literal();
    void string();
    uint8_t makeConstant(Value value);
    void parsePrecedence(Precedence precedence);
    ParseRule getRule(TokenType type);

private:
    Parser m_parser;
    Chunk m_compilingChunk;
    Scanner m_scanner;
    std::string& m_code;
    std::unordered_map<TokenType, ParseRule> m_rules = {
            {TokenType::LEFT_PAREN, {[this] { grouping(); }, nullptr, Precedence::NONE}},
            {TokenType::RIGHT_PAREN, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::LEFT_BRACE, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::RIGHT_BRACE, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::COMMA, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::DOT, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::MINUS, {[this] { unary(); }, [this] { binary(); }, Precedence::TERM}},
            {TokenType::PLUS, {nullptr, [this] { binary(); }, Precedence::TERM}},
            {TokenType::SEMICOLON, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::SLASH, {nullptr, [this] { binary(); }, Precedence::FACTOR}},
            {TokenType::STAR, {nullptr, [this] { binary(); }, Precedence::FACTOR}},
            {TokenType::BANG, {[this] { unary(); }, nullptr, Precedence::NONE}},
            {TokenType::BANG_EQUAL, {nullptr, [this] { binary(); }, Precedence::EQUALITY}},
            {TokenType::EQUAL, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::EQUAL_EQUAL, {nullptr, [this] { binary(); }, Precedence::EQUALITY}},
            {TokenType::GREATER, {nullptr, [this] { binary(); }, Precedence::COMPARISON}},
            {TokenType::GREATER_EQUAL, {nullptr, [this] { binary(); }, Precedence::COMPARISON}},
            {TokenType::LESS, {nullptr, [this] { binary(); }, Precedence::COMPARISON}},
            {TokenType::LESS_EQUAL, {nullptr, [this] { binary(); }, Precedence::COMPARISON}},
            {TokenType::IDENTIFIER, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::STRING, {[this] { string(); }, nullptr, Precedence::NONE}},
            {TokenType::NUMBER, {[this] { number(); }, nullptr, Precedence::NONE}},
            {TokenType::AND, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::CLASS, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::ELSE, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::FALSE, {[this] { literal(); }, nullptr, Precedence::NONE}},
            {TokenType::FOR, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::FUN, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::IF, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::NIL, {[this] { literal(); }, nullptr, Precedence::NONE}},
            {TokenType::OR, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::PRINT, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::RETURN, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::SUPER, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::THIS, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::TRUE, {[this] { literal(); }, nullptr, Precedence::NONE}},
            {TokenType::VAR, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::WHILE, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::ERROR, {nullptr, nullptr, Precedence::NONE}},
            {TokenType::TOKEN_EOF, {nullptr, nullptr, Precedence::NONE}},
    };
};
