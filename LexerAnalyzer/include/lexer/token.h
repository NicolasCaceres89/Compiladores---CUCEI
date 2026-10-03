#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum tokenType {
	MNEMONIC,	//0
	REGISTER,	//1
	NUMBER,		//2
	COMA,		//3
	UNKNOWN 	//4
};

class Token {
	public:
		//Attributes
		tokenType type;
		std::string lexeme;

		//Public Methods
		Token() : type(UNKNOWN), lexeme("") {}
		Token(tokenType type_, std::string lexeme_) : type(type_), lexeme(lexeme_) {}

		//Getters & Setters
		tokenType getType() const {	return this->type; }
		std::string getLexeme() const { return this->lexeme; }

		void setType(tokenType type_) { this->type = type_; }
		void setLexeme(std::string lexeme_) { this->lexeme = lexeme_; }

	private:
		//Private Methods
};

#endif