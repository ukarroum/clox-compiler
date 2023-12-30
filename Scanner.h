//
// Created by ykarroum on 12/27/23.
//

#ifndef CLOX_COMPILER_SCANNER_H
#define CLOX_COMPILER_SCANNER_H

#include <cstdlib>
#include <string>

enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR,

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    AND, CLASS, ELSE, FALSE,
    FOR, FUN, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS,
    TRUE, VAR, WHILE,

    ERROR, TOKEN_EOF
};

struct Token {
    TokenType type;
    size_t start;
    size_t length;
    size_t line;
    std::string errorMsg;
};

class Scanner {
public:
    explicit Scanner(std::string& code): m_code(code) {}
    Token scanToken();
private:
    Token makeToken(TokenType type);
    bool match(char expected);
    void skipWhitespaces();
    char peekNext();
    Token string();
    Token number();
    Token identifier();
    TokenType identifierType();
    TokenType checkKeyword(int start, const std::string& rest, TokenType type);

    std::string m_code;
    size_t m_start = 0;
    size_t m_current = 0;
    size_t m_line = 1;
};


#endif //CLOX_COMPILER_SCANNER_H
