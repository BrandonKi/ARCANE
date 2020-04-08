#include "VM.h"

#define POINTER(x) std::cout << "    POINTER: " << x << "\n";
#define INSTRUCTION(x) std::cout << "INSTRUCTION: " << x << "\n";
#define MNEMONIC(x) std::cout << "   MNEMONIC: " << x << "\n"
#define DEBUG(x) std::cout <<  " DEBUG INFO: " << x << "\n"

VM::VM(char* data, uint size)
	:m_size(size), m_memptr(0)
{
	m_data = convertToByteArray(data);
	m_variableTable = new VAR[10];
}


void VM::printProgram(byte* arr){
	std::cout << "PROGRAM: \n\t";
	for(uint i = 0; i < m_size; i++)
      std::cout << std::hex << (int)arr[i] << " ";
	NEWLINE;
	NEWLINE;
}

byte* VM::convertToByteArray(char* arr)
{
	byte* newArr;
	newArr = new byte[m_size];
	std::copy(arr, arr + m_size, newArr);
	return newArr;
}

void VM::run(){
	printProgram(m_data);
	while(m_memptr < m_size){
		POINTER(m_memptr);
		executeInstruction();
		nextInstruction();
	}
	NEWLINE;
	printStack();
}

void VM::nextInstruction(){
	m_memptr++;
}

byte* VM::getNextByte(){
	return &m_data[++m_memptr];
}

void VM::executeInstruction(){
	byte value = m_data[m_memptr];
	INSTRUCTION(std::hex << (int)value);
	switch(value){
		case 0x00:
			MNEMONIC("NOP");
			break;
		case 0x01:
			MNEMONIC("NULL");
			break;
		case 0xbb:
			MNEMONIC("PUSH_NEXT_BYTE");
			m_stack.push(getNextByte());
			DEBUG(std::hex << (int)*m_stack.top());
			break;
		case 0xaa:
			int result = *m_stack.top();
			m_stack.pop();
			result += *m_stack.top();
			m_stack.pop();
			MNEMONIC("ADD_TWO_BYTES");    // BEFORE: [byte], [byte]
			
			DEBUG(std::dec << result);
			break;
		// case 0x00:
		// 	std::cout << "NULL";
		// 	break;
		// case 0x00:
		// 	std::cout << "NULL";
		// 	break;
		// case 0x00:
		// 	std::cout << "NULL";
		// 	break;					
	}
	NEWLINE;
	NEWLINE;
}

void VM::printStack(){
	std::stack<byte*> temp = m_stack;
	std::stack<byte> result;
	while(!temp.empty()){
		result.push(*temp.top());
		temp.pop();
	}
	while(!result.empty()){
		std::cout << std::hex << (int)result.top() << "\n";
		result.pop();
	}
}