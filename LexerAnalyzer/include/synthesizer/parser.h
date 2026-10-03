#ifndef PARSER_H
#define PARSER_H

#include "../lexer/token.h"
#include "instruction.h"

class Parser {
    private:
        std::vector<Token> t;
        size_t currentToken;
        
        Token getCurrentToken();
        Token advance();
        bool checkTokenType(tokenType expectedType);

    public:
        Parser(const std::vector<Token>& streamTokens);
        instObject parseInstruction();

        ~Parser();
};

#endif