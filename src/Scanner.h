#ifndef LOX_COMPILER_SCANNER_H
#define LOX_COMPILER_SCANNER_H


#include <vector>
#include <unordered_map>
#include "Token.h"

class Scanner {
    std::string source;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> reservedWords;
    uint line = 1;
    uint start = 0;
    uint current = 0;

    bool isEnd();
public:
    explicit Scanner(std::string source);

    std::vector<Token> scanTokens();

    void scanToken();

    char parseNextChar();

    void addToken(TokenType type);

    void addToken(TokenType type, const Object& literal);

    void noMatchError() const;

    bool match(char expected);

    char peek();

    void parseString();

    static bool isDigit(char c);

    void parseNumber();

    char peekNext();

    static bool isAlphanumeric(char c);

    void parseIdentifier();
};


#endif //LOX_COMPILER_SCANNER_H
