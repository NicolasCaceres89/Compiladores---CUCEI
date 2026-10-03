#ifndef LEXER_MAP_H
#define LEXER_MAP_H

#include "token.h"
#include <map>

static const std::map<std::string, tokenType> lexerMap = {
    // List Mnemonics
    {"MOV", MNEMONIC},
    {"ADD", MNEMONIC},
    {"STO", MNEMONIC},
    {"END", MNEMONIC},

    // List Registers
    {"AL", REGISTER},
    {"AX", REGISTER},
    {"AH", REGISTER},
    {"BL", REGISTER},
    {"BH", REGISTER},

    // Coma
    {",", COMA}
};

#endif