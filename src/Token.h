#ifndef LOX_COMPILER_TOKEN_H
#define LOX_COMPILER_TOKEN_H

#include <string>
#include "Object.h"
#include "TokenType.h"

class Token {
public:
    TokenType type;
    std::string lexeme;
    Object literal;

    Token(TokenType type, std::string lexeme, Object literal);
    std::string toString() const;
};


#endif //LOX_COMPILER_TOKEN_H
