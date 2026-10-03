#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <vector>

enum class AddressingMode {
	None,
	Direct,
	Register
};

class instObject {
	public:
		//Attributes
		std::string mnemonic;
		std::vector<std::string> registers;
		int direction;
		AddressingMode addressingMode;

		//Methods
		instObject();		//Constructor
		instObject(const std::string&, std::vector<std::string>, int, AddressingMode);
		void printObject() const;	//Function to Print the Instruction Object
		~instObject();		//Destructor Object		
};

#endif