#include "../../include/lexer/afd.h"

#include <cctype>

//Logic Validation of Type of Lexeme in Current State...
lexemeState getLexemeState(lexemeState currentState, const char& c) {
	switch (currentState) {
		case Q_START:
			if (std::isalpha(static_cast<unsigned char>(c))) return Q_MNEMONIC_READ;
			if (std::isspace(static_cast<unsigned char>(c))) return Q_START;
			return Q_ERROR;
		case Q_MNEMONIC_READ:
			if (std::isalpha(static_cast<unsigned char>(c))) return Q_MNEMONIC_READ;
			if (std::isspace(static_cast<unsigned char>(c))) return Q_OPERATOR1_WAIT;
			return Q_ERROR;
		case Q_OPERATOR1_WAIT:
			if (std::isspace(static_cast<unsigned char>(c))) return Q_OPERATOR1_WAIT;
			if (std::isalpha(static_cast<unsigned char>(c))) return Q_OPERATOR1_READ;
			return Q_ERROR;
		case Q_OPERATOR1_READ:
			if (std::isalpha(static_cast<unsigned char>(c))) return Q_OPERATOR1_READ;
			if (c == ',') return Q_COMA;
			return Q_ERROR;
		case Q_COMA:
			if (std::isspace(static_cast<unsigned char>(c))) return Q_OPERATOR2_WAIT;
			if (std::isdigit(static_cast<unsigned char>(c))) return Q_OPERATOR2_READ;
			return Q_ERROR;
		case Q_OPERATOR2_WAIT:
			if (std::isspace(static_cast<unsigned char>(c))) return Q_OPERATOR2_WAIT;
			if (std::isdigit(static_cast<unsigned char>(c))) return Q_OPERATOR2_READ;
			return Q_ERROR;
		case Q_OPERATOR2_READ:
			if (std::isdigit(static_cast<unsigned char>(c))) return Q_OPERATOR2_READ;
			if (std::isspace(static_cast<unsigned char>(c))) return Q_END;
			return Q_ERROR;
		case Q_END:
			return std::isspace(static_cast<unsigned char>(c)) ? Q_END : Q_ERROR;
		case Q_ERROR:
			return Q_ERROR;
	}
	return Q_ERROR;
}

//Generate de Token using the Lexer Dictionary...
Token Tokenizer(std::string& buffer) {
	//Search the buffer result std::string for coincidence on LexerMap...
	auto mapResult = lexerMap.find(buffer);

	//If it is on the map, it save it...
	if (mapResult != lexerMap.end()) {
		return Token(mapResult->second, mapResult->first);
	}

	if (!buffer.empty()) {
		bool isNumber = true;
		for (const auto& c : buffer) {
			if (!std::isdigit(static_cast<unsigned char>(c))) {
				isNumber = false;
				break;
			}
		}

		if (isNumber)
			return Token(NUMBER, buffer);
	}

	return Token(UNKNOWN, buffer); //Return result of the search...
}

//Save each Token in a Stream Token...
std::vector<Token> generateTokens(std::string& input) {
	std::vector<Token> streamTokens;
	std::string buffer;
	for (const char c : input) {
		if (std::isalnum(static_cast<unsigned char>(c))) {
			buffer += c;
			continue;
		}

		if (!buffer.empty()) {
			streamTokens.push_back(Tokenizer(buffer));
			buffer.clear();
		}

		if (c == ',') {
			std::string comma = ",";
			streamTokens.push_back(Tokenizer(comma));
		} else if (!std::isspace(static_cast<unsigned char>(c))) {
			std::string unknown(1, c);
			streamTokens.push_back(Token(UNKNOWN, unknown));
		}
	}

	if (!buffer.empty()) {
		streamTokens.push_back(Tokenizer(buffer));
	}

	return streamTokens;
}

bool validateInstructionAFD(const std::vector<Token>& tokens, std::string& errorMessage) {
	errorMessage.clear();
	const auto reject = [&errorMessage](const std::string& message) {
		errorMessage = message;
		return false;
	};

	if (tokens.empty()) {
		return reject("falta el mnemónico de la instrucción.");
	}
	if (tokens.front().getType() != MNEMONIC) {
		return reject("el primer componente debe ser un mnemónico válido.");
	}

	const std::string& mnemonic = tokens.front().getLexeme();
	if (mnemonic == "END") {
		return tokens.size() == 1
				? true
				: reject("END no debe llevar operandos adicionales.");
	}
	if (mnemonic == "STO") {
		if (tokens.size() == 1) {
			return reject("falta la dirección después de STO.");
		}
		if (tokens[1].getType() != NUMBER) {
			return reject("STO requiere una dirección numérica.");
		}
		return tokens.size() == 2
				? true
				: reject("STO no debe llevar componentes adicionales.");
	}
	if (mnemonic != "MOV" && mnemonic != "ADD") {
		return reject("mnemónico no soportado: " + mnemonic + ".");
	}

	if (tokens.size() < 2 || tokens[1].getType() != REGISTER) {
		return reject("falta el registro destino después del mnemónico.");
	}
	if (tokens.size() < 3) {
		return reject("falta la coma entre el registro y el operando.");
	}
	if (tokens[2].getType() != COMA) {
		if (mnemonic == "MOV" && tokens[2].getType() == NUMBER) {
			return reject("falta la coma entre el registro y la dirección.");
		}
		if (mnemonic == "ADD" && tokens[2].getType() == REGISTER) {
			return reject("falta la coma entre los registros.");
		}
		return reject("se esperaba una coma después del registro destino.");
	}
	if (tokens.size() < 4) {
		return reject(mnemonic == "ADD"
				? "falta el registro fuente después de la coma."
				: "falta la dirección o el registro fuente después de la coma.");
	}
	if (tokens.size() > 4) {
		return reject("hay componentes adicionales después del operando.");
	}
	if (mnemonic == "MOV") {
		return tokens[3].getType() == REGISTER || tokens[3].getType() == NUMBER
				? true
				: reject("el operando de MOV debe ser una dirección numérica o un registro.");
	}
	return tokens[3].getType() == REGISTER
			? true
			: reject("el operando fuente de ADD debe ser un registro.");
}