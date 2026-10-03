#include "lexer/afd.h"
#include "synthesizer/parser.h"
#include "control_unit/moore_fsm.h"

#include <iostream>
#include <string>
#include <vector>

namespace {
const char* tokenTypeName(tokenType type) {
	switch (type) {
		case MNEMONIC: return "MNEMONICO";
		case REGISTER: return "REGISTRO";
		case COMA: return "COMA";
		case NUMBER: return "NUMERO";
		case UNKNOWN: return "DESCONOCIDO";
	}
	return "DESCONOCIDO";
}
}

int main() {
	std::string input;
	if (!std::getline(std::cin, input)) {
		std::cerr << "Error: no se recibió una instrucción.\n";
		return 1;
	}

	std::vector<Token> tokens = generateTokens(input);
	std::cout << "ENTRADA\n" << input << "\n";
	std::cout << "VALIDACIÓN MEDIANTE AFD\n";
	std::string validationError;
	if (!validateInstructionAFD(tokens, validationError)) {
		std::cout << "Instrucción inválida: " << validationError << '\n';
		std::cout << "No se construye el objeto instrucción.\n";
		std::cout << "No se generan microoperaciones.\n";
		return 1;
	}
	std::cout << "Instrucción válida.\n";

	std::cout << "TOKENS RECONOCIDOS\n";
	for (const Token& token : tokens) {
		const std::string typeName = token.getType() == MNEMONIC
				? token.getLexeme()
				: tokenTypeName(token.getType());
		std::cout << typeName << "(\"" << token.getLexeme() << "\")\n";
	}

	try {
		Parser parser(tokens);
		const instObject instruction = parser.parseInstruction();
		std::cout << "COMPONENTES IDENTIFICADOS\n";
		std::cout << "Mnemónico: " << instruction.mnemonic << '\n';
		if (!instruction.registers.empty()) {
			std::cout << "Registro destino: " << instruction.registers.front() << '\n';
		}
		if (instruction.mnemonic == "STO") {
			std::cout << "Dirección de memoria: " << instruction.direction << '\n';
			std::cout << "Direccionamiento: directo\n";
		} else if (instruction.addressingMode == AddressingMode::Direct) {
			std::cout << "Dirección de memoria: " << instruction.direction << '\n';
			std::cout << "Direccionamiento: directo\n";
		} else if (instruction.addressingMode == AddressingMode::Register) {
			std::cout << "Registro fuente: " << instruction.registers[1] << '\n';
			std::cout << "Direccionamiento: por registro\n";
		}
		instruction.printObject();

		std::cout << "MICROOPERACIONES GENERADAS POR MOORE\n";
		MooreFSM machine(instruction);
		MicroOp microOperation;
		std::size_t step = 1;
		while (machine.siguientePaso(microOperation)) {
			std::cout << step++ << ". " << microOperation.description << '\n';
		}
		std::cout << "Generación terminada.\n";
	} catch (const std::exception& error) {
		std::cerr << error.what() << '\n';
		return 1;
	}

	return 0;
}