//
// Created by ykarroum on 12/27/23.
//

#include <stdexcept>

#include "Scanner.h"

Token Scanner::scanToken() {
    skipWhitespaces();

    m_start = m_current;

    if(m_current == m_code.size())
    {
        return makeToken(TokenType::TOKEN_EOF);
    }

    char c = advance();

    if(std::isalpha(c))
        return identifier();
    if(std::isdigit(c))
        return number();

    switch(c)
    {
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case ';': return makeToken(TokenType::SEMICOLON);
        case ',': return makeToken(TokenType::COMMA);
        case '.': return makeToken(TokenType::DOT);
        case '-': return makeToken(TokenType::MINUS);
        case '+': return makeToken(TokenType::PLUS);
        case '/': return makeToken(TokenType::SLASH);
        case '*': return makeToken(TokenType::STAR);
        case '!': return makeToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        case '=': return makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        case '<': return makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        case '>': return makeToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        case '"': return string();
    }

    return {TokenType::ERROR, 0, 0, 0};
}

Token Scanner::makeToken(TokenType type) {
    return {type, m_start, m_current - m_start, m_line};
}

char Scanner::advance() {
    return m_code[m_current++];
}

bool Scanner::match(char expected) {
    if(m_current == m_code.size())
        return false;
    if(m_code[m_current] != expected)
        return false;

    m_current++;
    return true;
}

void Scanner::skipWhitespaces() {
    while(true){
        switch(m_code[m_current]) {
            case '\n':
                m_line++;
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '/':
                if(peekNext() == '/')
                    while (m_code[m_current] != '\n' && (!m_current < m_code.size())) advance();
                else
                    return;
                break;
            default:
                return;
        }
    }

}

char Scanner::peekNext() {
    if(m_current == m_code.size())
        return 0;
    return m_code[m_current + 1];
}

Token Scanner::string() {
    while(m_code[m_current] != '"' && m_current < m_code.size())
    {
        if(m_code[m_current] == '\n')
            m_line++;
        advance();
    }

    if(m_current < m_code.size())
        throw std::runtime_error("Unterminated string");

    advance();
    return makeToken(TokenType::STRING);
}

Token Scanner::number() {
    while(std::isdigit(m_code[m_current]))
        advance();

    if(m_code[m_current] == '.' && std::isdigit(peekNext()))
    {
        advance();

        while(std::isdigit(m_code[m_current])) advance();
    }

    return makeToken(TokenType::NUMBER);
}

Token Scanner::identifier() {
    while(std::isalpha(m_code[m_current]) || std::isdigit(m_code[m_current])) advance();

    return makeToken(identifierType());
}

TokenType Scanner::identifierType() {
    switch(m_code[m_start]) {
        case 'a': return checkKeyword(1, "nd", TokenType::AND);
        case 'c': return checkKeyword(1, "lass", TokenType::CLASS);
        case 'e': return checkKeyword(1, "lse", TokenType::ELSE);
        case 'i': return checkKeyword(1, "f", TokenType::IF);
        case 'n': return checkKeyword(1, "il", TokenType::NIL);
        case 'o': return checkKeyword(1, "r", TokenType::OR);
        case 'p': return checkKeyword(1, "rint", TokenType::PRINT);
        case 'r': return checkKeyword(1, "eturn", TokenType::RETURN);
        case 's': return checkKeyword(1, "uper", TokenType::SUPER);
        case 'v': return checkKeyword(1, "ar", TokenType::VAR);
        case 'w': return checkKeyword(1, "hile", TokenType::WHILE);
        case 'f':
            if(m_current - m_start > 1){
                switch(m_code[m_start]) {
                    case 'a': return checkKeyword(2, "lse", TokenType::FALSE);
                    case 'o': return checkKeyword(2, "r", TokenType::FOR);
                    case 'u': return checkKeyword(2, "n", TokenType::FUN);
                }
            }
            break;
        case 't':
            if(m_current - m_start > 1){
                switch(m_code[m_start]) {
                    case 'h': return checkKeyword(2, "is", TokenType::THIS);
                    case 'r': return checkKeyword(2, "ue", TokenType::TRUE);
                }
            }
    }
}

TokenType Scanner::checkKeyword(int start, const std::string &rest, TokenType type) {
    if(m_current - m_start == start + rest.size() && m_code.substr(m_start + start, rest.size()) == rest)
        return type;
    return TokenType::IDENTIFIER;
}

