#include "VM.h"

VM::VM(char* data, uint size)
	:m_size(size), m_memptr(0)
{
	m_data = reinterpret_cast<byte*>(data);
	// m_data = convertToByteArray(data);

	// VAR m_variableTable[10];
	// std::cout << m_variableTable[0].type;
	// store(Type::UNDEFINED, 0, 0);
	// store(Type::UNDEFINED, 0, 1);
	// store(Type::UNDEFINED, 0, 2);
	// store(Type::UNDEFINED, 0, 3);
	// store(Type::UNDEFINED, 0, 4);
	// store(Type::UNDEFINED, 0, 5);
	// store(Type::UNDEFINED, 0, 6);
	// store(Type::UNDEFINED, 0, 7);
	// store(Type::UNDEFINED, 0, 8);
	// store(Type::UNDEFINED, 0, 9);
	// printVariableTable();
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

	while(!EXIT){
		// std::cin.get();
		POINTER(std::dec << m_memptr);
		executeInstruction();
		nextInstruction();
		
	}


	// int x = Type::UNDEFINED;
	// logn(x);


	// unsigned char temp[] = {0x11, 0x00, 0x00, 0x00};
	// uint* tempptr = reinterpret_cast<uint*>(temp);
	// logn((uint)*tempptr);

	// logn(std::hex << (int)m_data[0] << " " << (int)m_data[1] << " " << (int)m_data[2] << " " << (int)m_data[3]);
	// logn(std::dec << (int)m_data[0] << " " << (int)m_data[1] << " " << (int)m_data[2] << " " << (int)m_data[3]);
	// int data = reinterpret_cast<int>(getNextFourBytes());
	// logn(std::dec << data);
	// NEWLINE;
	// printStack();
	// NEWLINE;
	// printTempStack();
	// NEWLINE;
	//printVariableTable();
	// EXIT(std::dec << EXIT_CODE);
}

void VM::nextInstruction(){
	m_memptr++;
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
			NCONST_PUSH();
			break;
		}
		case 0x10:
		{
			MNEMONIC("SBCONST_PUSH");
			m_tempVarStorage.push(*getNextByte());
			DEBUG(std::dec << (int)m_tempVarStorage.top());
			break;
		}
		case 0x11:
		{
			MNEMONIC("UBCONST_PUSH");
			m_tempVarStorage.push((uint)*getNextByte());
			DEBUG(std::dec << (uint)m_tempVarStorage.top());
			break;
		}
		case 0x12:
		{
			MNEMONIC("SICONST_PUSH");
			int* tempptr = reinterpret_cast<int*>(&m_data[m_memptr]);
			DEBUG(*tempptr);
			m_tempVarStorage.push(getNextFourBytes());
			int temp = *reinterpret_cast<int*>(&m_tempVarStorage.top());
			DEBUG(std::dec << temp);
			break;
		}
		case 0x13:
		{
			MNEMONIC("UICONST_PUSH");
			m_tempVarStorage.push(getNextFourBytes());
			uint temp = *reinterpret_cast<uint*>(&m_tempVarStorage.top());
			DEBUG(std::dec << temp);
			break;
		}
		case 0xa0:
		{
			MNEMONIC("SB_ADD");    // BEFORE: [byte], [byte]
			uint result = m_tempVarStorage.top();
			m_tempVarStorage.pop();
			result += m_tempVarStorage.top();
			m_tempVarStorage.pop();
			DEBUG(std::dec << result);
			m_tempVarStorage.push(result);
			break;
		}
		case 0xa1:
		{
			MNEMONIC("UB_ADD");    // BEFORE: [byte], [byte]
			uint result = m_tempVarStorage.top();
			m_tempVarStorage.pop();
			result += m_tempVarStorage.top();
			m_tempVarStorage.pop();
			DEBUG(std::dec << result);
			m_tempVarStorage.push(result);
			break;
		}
		case 0xa2:
		{
			MNEMONIC("SI_ADD");    // BEFORE: [int], [int]
			int result = m_tempVarStorage.top();
			m_tempVarStorage.pop();
			result += m_tempVarStorage.top();
			m_tempVarStorage.pop();
			DEBUG(std::dec << result);
			m_tempVarStorage.push(result);
			break;
		}
		case 0xa3:
		{
			MNEMONIC("UI_ADD");    // BEFORE: [int], [int]
			uint result = m_tempVarStorage.top();
			m_tempVarStorage.pop();
			result += m_tempVarStorage.top();
			m_tempVarStorage.pop();
			DEBUG(std::dec << result);
			m_tempVarStorage.push(result);
			break;
		}
		case 0xa4:
		{
			MNEMONIC("F_ADD");    // BEFORE: [float], [float]
			float result = m_tempVarStorage.top();
			m_tempVarStorage.pop();
			result += m_tempVarStorage.top();
			m_tempVarStorage.pop();
			DEBUG(std::dec << result);
			m_tempVarStorage.push(result);
			break;
		}
		case 0xa5:
		{
			MNEMONIC("D_ADD");    // BEFORE: [double], [double]
			double result = m_tempVarStorage.top();
			m_tempVarStorage.pop();
			result += m_tempVarStorage.top();
			m_tempVarStorage.pop();
			DEBUG(std::dec << result);
			m_tempVarStorage.push(result);
			break;
		}
		case 0xc0:
		{
			MNEMONIC("STORE_SUM_TWO_BYTES_SIGNED");    // BEFORE: [byte], [byte]    AFTER: [slot number] 
			uint result = m_tempVarStorage.top();
			m_tempVarStorage.pop();
			result += m_tempVarStorage.top();
			m_tempVarStorage.pop();
			byte index = *getNextByte();
			DEBUG(std::dec << "Index " << (int)index);
			DEBUG(std::dec << m_variableTable[(int)index].type);
			store(Type(CHAR), result, index);
			DEBUG(std::hex << (int)m_variableTable[index].c);
			break;
		}
		case 0xc1:
		{
			MNEMONIC("STORE_SUM_TWO_BYTES_UNSIGNED");    // BEFORE: [byte], [byte]    AFTER: [slot number] 
			uint result = m_tempVarStorage.top();
			m_tempVarStorage.pop();
			result += m_tempVarStorage.top();
			m_tempVarStorage.pop();
			DEBUG(std::dec << result);
			store(Type(UCHAR), result, *getNextByte());
			log(m_variableTable[0].uc);
			break;
		}
		
		case 0xee:
		{
			m_data[m_memptr] = 0x00;
			m_data[m_memptr+1] = 0x00;
			byte address = *getNextByte();
			MNEMONIC("GOTO" << address);
			m_memptr = address;
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
	if(m_memptr >= m_size){
		EXIT_ON_NEXT_INTSRUCTION = true;
		EXIT_CODE = 1;
	}
}

void VM::printTempStack(){
	std::stack<uint> temp = m_tempVarStorage;
	std::stack<uint> result;
	while(!temp.empty()){
		result.push(temp.top());
		temp.pop();
	}
	while(!result.empty()){
		std::cout << std::dec << (int)result.top() << "\n";
		result.pop();
	}
}

void VM::printVariableTable(){
	log(std::dec);
	for(int i = 0; i < 10; i++){
		logn(m_variableTable[i].type);
		switch(m_variableTable[i].type){
			case BOOL:
			{
				logn((int)m_variableTable[i].type << "|" << m_variableTable[i].b);
				break;
			}
			case INT:
			{
				logn((int)m_variableTable[i].type << "|" << (int)m_variableTable[i].i);
				break;
			}
			case UINT:
			{
				logn((int)m_variableTable[i].type << "|" << (uint)m_variableTable[i].ui);
				break;
			}
			case CHAR:
			{
				logn((int)m_variableTable[i].type << "|" << (int)m_variableTable[i].c);
				break;
			}
			case UCHAR:
			{
				logn((int)m_variableTable[i].type << "|" << (int)m_variableTable[i].uc);
				break;
			}
			case FLOAT:
			{
				logn((int)m_variableTable[i].type << "|" << (float)m_variableTable[i].f);
				break;
			}	
			case DOUBLE:
			{
				logn((int)m_variableTable[i].type << "|" << (double)m_variableTable[i].d);
				break;
			}
		}
	}
}

void VM::store(Type type, uint val, byte index){
	m_variableTable[index].type = type;
	switch(type){
		case UNDEFINED:
		{
			m_variableTable[(int)index].u = val;
			break;
		}
		case BOOL:
		{
			m_variableTable[(int)index].b = (bool)val;
			break;
		}
		case INT:
		{
			m_variableTable[(int)index].i = (int)val;
			break;
		}
		case UINT:
		{
			m_variableTable[(int)index].ui = (uint)val;
			break;
		}
		case CHAR:
		{
			m_variableTable[(int)index].c = (char)val;
			break;
		}
		case UCHAR:
		{
			m_variableTable[(int)index].uc = (unsigned char)val;
			break;
		}
		case FLOAT:
		{
			m_variableTable[(int)index].f = (float)val;
			break;
		}	
		case DOUBLE:
		{
			m_variableTable[(int)index].d = (double)val;
			break;
		}
	}

}
