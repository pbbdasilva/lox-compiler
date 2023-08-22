#include <iostream>
#include "Scanner.h"

Scanner::Scanner(std::string source) {
    this->source = std::move(source);

    reservedWords.insert(std::make_pair<std::string, TokenType>("and", AND));
    reservedWords.insert(std::make_pair<std::string, TokenType>("class", CLASS));
    reservedWords.insert(std::make_pair<std::string, TokenType>("else", ELSE));
    reservedWords.insert(std::make_pair<std::string, TokenType>("false", FALSE));
    reservedWords.insert(std::make_pair<std::string, TokenType>("for", FOR));
    reservedWords.insert(std::make_pair<std::string, TokenType>("fun", FUN));
    reservedWords.insert(std::make_pair<std::string, TokenType>("if", IF));
    reservedWords.insert(std::make_pair<std::string, TokenType>("nil", NIL));
    reservedWords.insert(std::make_pair<std::string, TokenType>("or", OR));
    reservedWords.insert(std::make_pair<std::string, TokenType>("print", PRINT));
    reservedWords.insert(std::make_pair<std::string, TokenType>("return", RETURN));
    reservedWords.insert(std::make_pair<std::string, TokenType>("super", SUPER));
    reservedWords.insert(std::make_pair<std::string, TokenType>("this", THIS));
    reservedWords.insert(std::make_pair<std::string, TokenType>("true", TRUE));
    reservedWords.insert(std::make_pair<std::string, TokenType>("var", VAR));
    reservedWords.insert(std::make_pair<std::string, TokenType>("while", WHILE));
}

std::vector <Token> Scanner::scanTokens() {
    while(!isEnd()) {
        start = current;
        scanToken();
    }

    return tokens;
}

bool Scanner::isEnd() {
    return current >= source.size();
}

void Scanner::scanToken() {
    char c = parseNextChar();

    switch (c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;

        case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
        case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
        case '<': addToken(match('=') ? LESS_EQUAL : EQUAL); break;
        case '>': addToken(match('=') ? GREATER_EQUAL : EQUAL); break;

        case '/':
            if(match('/')) {
                while(peek() != '\n' and !isEnd()) parseNextChar();
            } else addToken(SLASH);
            break;

        case ' ':
        case '\r':
        case '\t': break;

        case '\n': line++; break;
        case '"': parseString(); break;
        default:
            if(isDigit(c)) {
                parseNumber();
            } else if(isAlphanumeric(c)) {
                parseIdentifier();
            } else noMatchError();
    }
}

char Scanner::parseNextChar() {
    return source[current++];
}

void Scanner::addToken(TokenType type) {
    std::string txt = source.substr(start, current - start + 1);
    tokens.emplace_back(type, txt, "undefined");
}

void Scanner::addToken(TokenType type, const Object& literal) {
    tokens.emplace_back(type, source.substr(start, current - start + 1), literal);
}

void Scanner::noMatchError() const {
    std::cout << "token did not match any value at line " << line;
}

bool Scanner::match(char expected) {
    if(isEnd() or source[current] != expected) return false;
    current++;
    return true;
}

char Scanner::peek() {
    if(isEnd()) return '\0';
    return source[current];
}

char Scanner::peekNext() {
    if(current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

void Scanner::parseString() {
    while(peek() != '"' and !isEnd()) {
        if(peek() == '\n') line++;
        parseNextChar();
    }

    if(isEnd()) {
        noMatchError();
        return;
    }

    uint len = current - start - 1;
    addToken(STRING, source.substr(start + 1, len));
    parseNextChar();
}

bool Scanner::isDigit(char c) {
    return c >= '0' and c <= '9';
}

void Scanner::parseNumber() {
    while(isDigit(peek()) and !isEnd()) parseNextChar();

    if(peek() == '.' and isDigit(peekNext())) {
        parseNextChar();
        while (isDigit(peek()) and !isEnd()) parseNextChar();
    }

    addToken(NUMBER,std::stod(source.substr(start, current - start + 1)));
}

bool Scanner::isAlphanumeric(char c) {
    return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') or c == '_';
}

void Scanner::parseIdentifier() {
    while(isAlphanumeric(peek()) or isDigit(peek())) parseNextChar();
    std::string text = source.substr(start, current - start + 1);
    auto itr = reservedWords.find(text);
    auto type = itr == reservedWords.end() ? IDENTIFIER : itr->second;
    addToken(type);
}
