#include "instruction.h"

#include <utility>

//Base Constructor
instObject::instObject() : mnemonic(""), direction(-1), addressingMode(AddressingMode::None) {}

//Constructor Assign
instObject::instObject(const std::string& mnemonic_, std::vector<std::string> registers_,
		int direction_, AddressingMode addressingMode_)
		: mnemonic(mnemonic_), registers(std::move(registers_)), direction(direction_),
		  addressingMode(addressingMode_) {}

void instObject::printObject() const {
    std::cout << "OBJETO INSTRUCCIÓN\n{\n";
        std::cout << "operacion: \"" << mnemonic << "\",\n";
        
        std::cout << "registros: [";
        for (size_t i = 0; i < registers.size(); ++i) {
            std::cout << "\"" << registers[i] << "\"";
            if (i + 1 < registers.size()) std::cout << ", ";
        }
        std::cout << "],\n";

        std::cout << "direccion: ";
        if (direction != -1) {
            std::cout << direction << "\n";
        } else {
            std::cout << "null\n";
        }
        std::cout << "}\n";
}

instObject::~instObject() {
	//If it need logic, will go here...
}