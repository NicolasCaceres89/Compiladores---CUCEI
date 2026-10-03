#include "parser.h"

#include <stdexcept>
#include <utility>

Parser::Parser(const std::vector<Token>& streamTokens)
    : t(streamTokens), currentToken(0) {}

Token Parser::getCurrentToken() {
    if (currentToken < t.size()) {
        return t[currentToken];
    }

    return Token(UNKNOWN, "");
}

Token Parser::advance() {
    Token current = getCurrentToken();
    if (currentToken < t.size()) {
        ++currentToken;
    }

    return current;
}

bool Parser::checkTokenType(tokenType expectedType) {
    return getCurrentToken().getType() == expectedType;
}

instObject Parser::parseInstruction() {
    if (!checkTokenType(MNEMONIC)) {
        throw std::invalid_argument("Error sintáctico: se esperaba un mnemónico.");
    }
    const std::string mnemonic = advance().getLexeme();

    if (mnemonic == "END") {
        if (currentToken != t.size()) {
            throw std::invalid_argument("Error sintáctico: END no acepta operandos.");
        }
        return instObject(mnemonic, {}, -1, AddressingMode::None);
    }

    if (mnemonic == "STO") {
        if (!checkTokenType(NUMBER)) {
            throw std::invalid_argument("Error sintáctico: STO requiere una dirección numérica.");
        }
        const std::string address = advance().getLexeme();
        if (currentToken != t.size()) {
            throw std::invalid_argument("Error sintáctico: hay tokens adicionales después de STO.");
        }
        std::size_t parsedCharacters = 0;
        const int direction = std::stoi(address, &parsedCharacters);
        if (parsedCharacters != address.size()) {
            throw std::invalid_argument("Error sintáctico: la dirección no es un número válido.");
        }
        return instObject(mnemonic, {}, direction, AddressingMode::Direct);
    }

    if (mnemonic != "MOV" && mnemonic != "ADD") {
        throw std::invalid_argument("Error sintáctico: mnemónico no soportado.");
    }

    if (!checkTokenType(REGISTER)) {
        throw std::invalid_argument("Error sintáctico: se esperaba un registro destino.");
    }
    std::vector<std::string> registers{advance().getLexeme()};

    if (!checkTokenType(COMA)) {
        throw std::invalid_argument("Error sintáctico: se esperaba una coma.");
    }
    advance();

    int direction = -1;
    AddressingMode addressingMode;
    if (checkTokenType(NUMBER) && mnemonic == "MOV") {
        const std::string directionLexeme = advance().getLexeme();
        std::size_t parsedCharacters = 0;
        direction = std::stoi(directionLexeme, &parsedCharacters);
        if (parsedCharacters != directionLexeme.size()) {
            throw std::invalid_argument("Error sintáctico: la dirección no es un número válido.");
        }
        addressingMode = AddressingMode::Direct;
    } else if (checkTokenType(REGISTER)) {
        registers.push_back(advance().getLexeme());
        addressingMode = AddressingMode::Register;
    } else {
        throw std::invalid_argument(mnemonic == "ADD"
                ? "Error sintáctico: ADD requiere un registro fuente."
                : "Error sintáctico: MOV requiere una dirección numérica o un registro fuente.");
    }

    if (currentToken != t.size()) {
        throw std::invalid_argument("Error sintáctico: hay tokens adicionales después de la instrucción.");
    }

    return instObject(mnemonic, std::move(registers), direction, addressingMode);
}

Parser::~Parser() = default;
