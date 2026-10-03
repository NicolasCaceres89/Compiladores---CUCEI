#ifndef MOORE_FSM_H
#define MOORE_FSM_H

#include "micro_op.h"
#include "../synthesizer/instruction.h"

#include <string>

class MooreFSM {
	public:
		explicit MooreFSM(const instObject& instruction);
		bool siguientePaso(MicroOp& output);

	private:
		enum class State {
			PrepareAddress,
			ReadMemory,
			LoadRegister,
			MoveRegister,
			AddRegisters,
			StoreData,
			WriteMemory,
			Finish
		};

		instObject instruction;
		State currentState;
};

#endif