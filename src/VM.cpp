#include "VM.h"

VM::VM(char* data, uint size)
	:m_size(size), m_memptr(0)
{
	m_data = reinterpret_cast<byte*>(data);
	m_variableTable = (uint*)malloc(10);  // 


	// m_data = convertToByteArray(data);  // outdated

	// printVariableTable();
}


void VM::printProgram(byte* arr){
	std::cout << "PROGRAM: \n\t";
	for(uint i = 0; i < m_size; i++)
      std::cout << std::hex << (int)arr[i] << " ";
	NEWLINE;
	NEWLINE;
}

byte* VM::convertToByteArray(char* arr)             // USELESS
{
	byte* newArr = new byte [m_size];
	std::copy(arr, arr + ((m_size+1) * sizeof(char)), newArr);
	return newArr;
}

void VM::run(){
	// store(Type(CHAR), 0x13, 0);
	// store(Type(CHAR), 0x55, 1);
	// store(Type(CHAR), 0x21, 2);
	// store(Type(CHAR), 0x07, 3);
	// printProgram(m_data);

	while(!EXIT_ON_NEXT_INTSRUCTION){
		std::cin.get();
		POINTER(std::dec << m_memptr);
		executeInstruction();
		nextInstruction();
		printStack();
	}


	// int x = Type::UNDEFINED;
	// logn(x);


	// unsigned char temp[] = {0xee, 0xee, 0xee, 0xee}; // Little Endian
	// uint* tempptr = reinterpret_cast<uint*>(temp);						Example version
	// logn((uint)*tempptr);

	// uint tempptr2 = *reinterpret_cast<uint*>(&m_data[0]);    			Usable version

	EXIT_MSG(std::dec << EXIT_CODE);
}

void VM::nextInstruction(){
	m_memptr++;
	if(m_memptr > m_size){
		EXIT_ON_NEXT_INTSRUCTION = true;
		EXIT_CODE = 1;
	}
}

byte* VM::getNextByte(){
	return &m_data[++m_memptr];
}

uint VM::getNextFourBytes(){
	return reinterpret_cast<uint>(((uint)m_data[++m_memptr]<<24) | ((uint)m_data[++m_memptr]<<16) | ((uint)m_data[++m_memptr]<<8) | (uint)m_data[++m_memptr]);
}

void VM::executeInstruction(){
	byte value = m_data[m_memptr];
	INSTRUCTION("0x" << std::hex << (int)value);
	log(std::dec);
	switch(value){
		case 0x00:
		{
			NOP();
			break;
		}
		case 0x01:
		{
			EXIT();
			break;
		}
		case 0x02:
		{
			NCONST_PUSH();   // This has no implementation
			break;
		}
		case 0x10:
		{
			SBCONST_PUSH();
			break;
		}
		case 0x11:
		{
			UBCONST_PUSH();
			break;
		}
		case 0x12:
		{
			SICONST_PUSH();
			break;
		}
		case 0x13:
		{
			UICONST_PUSH();
			break;
		}
		case 0x14:
		{
			FCONST_PUSH();
			break;
		}
		case 0x15:
		{
			DCONST_PUSH();
			break;
		}
		case 0xa0:
		{
			SB_ADD();
			break;
		}
		case 0xa1:
		{
			UB_ADD();
			break;
		}
		case 0xa2:
		{
			SI_ADD();
			break;
		}
		case 0xa3:
		{
			UI_ADD();
			break;
		}
		case 0xa4:
		{
			F_ADD();
			break;
		}
		case 0xa5:
		{
			D_ADD();
			break;
		}
		case 0xc0:
		{
			SB_STORE();
			break;
		}
		case 0xc1:
		{
			UB_STORE();
			break;
		}
		
		case 0xee:
		{
			GOTO();
			break;
		}

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
	std::stack<uint> temp = m_stack;
	std::stack<uint> result;
	while(!temp.empty()){
		result.push(temp.top());
		temp.pop();
	}
	while(!result.empty()){
		std::cout << std::dec << (uint)result.top() << "\n";
		result.pop();
	}
}

void VM::printVariableTable(){

}