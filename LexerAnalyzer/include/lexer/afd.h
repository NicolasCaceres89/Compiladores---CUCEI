#ifndef AFD_H
#define AFD_H

#include "lexer_maps.h"
#include "token.h"
#include <string>
#include <vector>

//AFD Logic Process
enum lexemeState {
	Q_START,
	Q_MNEMONIC_READ,
	Q_OPERATOR1_WAIT,
	Q_OPERATOR1_READ,
	Q_COMA,
	Q_OPERATOR2_WAIT,
	Q_OPERATOR2_READ,
	Q_END,
	Q_ERROR
};

//Validate what type of Lexeme is the state...
lexemeState getLexemeState(lexemeState currentState, const char& c);

//Assign the corresponding token to the string...
Token Tokenizer(std::string& buffer);

//Save the Token's into a vector...
std::vector<Token> generateTokens(std::string&);

//Validate the instruction grammar using the token-level finite automaton.
bool validateInstructionAFD(const std::vector<Token>& tokens, std::string& errorMessage);

#endif