#include "moore_fsm.h"

#include <stdexcept>

MooreFSM::MooreFSM(const instObject& instruction_)
		: instruction(instruction_), currentState(State::Finish) {
	if (instruction.mnemonic == "END" &&
			instruction.addressingMode == AddressingMode::None) {
		currentState = State::Finish;
	} else if (instruction.mnemonic == "MOV" &&
			instruction.addressingMode == AddressingMode::Direct &&
			instruction.registers.size() == 1 && instruction.direction >= 0) {
		currentState = State::PrepareAddress;
	} else if (instruction.mnemonic == "MOV" &&
			instruction.addressingMode == AddressingMode::Register &&
			instruction.registers.size() == 2) {
		currentState = State::MoveRegister;
	} else if (instruction.mnemonic == "ADD" &&
			instruction.addressingMode == AddressingMode::Register &&
			instruction.registers.size() == 2) {
		currentState = State::AddRegisters;
	} else if (instruction.mnemonic == "STO" &&
			instruction.addressingMode == AddressingMode::Direct &&
			instruction.direction >= 0 && instruction.registers.empty()) {
		currentState = State::PrepareAddress;
	} else {
		throw std::invalid_argument("No hay una ruta de Moore definida para esta instrucción.");
	}
}

bool MooreFSM::siguientePaso(MicroOp& output) {
	switch (currentState) {
		case State::PrepareAddress:
			output.description = "MAR ← " + std::to_string(instruction.direction);
			currentState = instruction.mnemonic == "STO"
					? State::StoreData : State::ReadMemory;
			return true;
		case State::ReadMemory:
			output.description = "MBR ← M[MAR]";
			currentState = State::LoadRegister;
			return true;
		case State::LoadRegister:
			output.description = instruction.registers.front() + " ← MBR";
			currentState = State::Finish;
			return true;
		case State::MoveRegister:
			output.description = instruction.registers[0] + " ← " + instruction.registers[1];
			currentState = State::Finish;
			return true;
		case State::AddRegisters:
			output.description = instruction.registers[0] + " ← " +
					instruction.registers[0] + " + " + instruction.registers[1];
			currentState = State::Finish;
			return true;
		case State::StoreData:
			output.description = "MBR ← AL";
			currentState = State::WriteMemory;
			return true;
		case State::WriteMemory:
			output.description = "M[MAR] ← MBR";
			currentState = State::Finish;
			return true;
		case State::Finish:
			return false;
	}
	return false;
}