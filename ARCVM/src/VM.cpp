#include "VM.h"

VM::VM(char* data, uint size)
	:m_size(size), m_memptr(0)
{
	
	m_data = reinterpret_cast<byte*>(data);
	m_variableTable = (container*)malloc(10);  

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

	while(!EXIT_ON_NEXT_INTSRUCTION){
		WAIT_FOR_INPUT;
		POINTER(std::dec << m_memptr);
		executeInstruction();
		nextInstruction();
		DEBUG_STACK;
	}

	// unsigned char temp[] = {0xaa, 0xee, 0xee, 0xee}; // Little Endian
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
		case _NOP_:
			NOP();
			break;
		case _EXIT_:
			EXIT();
			break;
		case _NCONST_PUSH_:
			NCONST_PUSH();
			break;
		case _SBCONST_PUSH_:
			SBCONST_PUSH();
			break;
		case _UBCONST_PUSH_:
			UBCONST_PUSH();
			break;
		case _SICONST_PUSH_:
			SICONST_PUSH();
			break;
		case _UICONST_PUSH_:
			UICONST_PUSH();
			break;
		case _FCONST_PUSH_:
			FCONST_PUSH();
			break;
		case _DCONST_PUSH_:
			DCONST_PUSH();
			break;
		case _LCONST_PUSH_:
			LCONST_PUSH();
			break;
		case _SCONST_PUSH_:
			SCONST_PUSH();
			break;
		case _NEW_ARR_:
			NEW_ARR();
			break;
		case _UBA_STORE_:
			UBA_STORE();
			break;
		case _UBA_LOAD_:
			UBA_LOAD();
			break;
		case _SB_ADD_:
			SB_ADD();
			break;
		case _UB_ADD_:
			UB_ADD();
			break;
		case _SI_ADD_:
			SI_ADD();
			break;
		case _UI_ADD_:
			UI_ADD();
			break;
		case _F_ADD_:
			F_ADD();
			break;
		case _D_ADD_:
			D_ADD();
			break;
		case _L_ADD_:
			L_ADD();
			break;
		case _SB_SUB_:
			SB_SUB();
			break;
		case _UB_SUB_:
			UB_SUB();
			break;
		case _SI_SUB_:
			SI_SUB();
			break;
		case _UI_SUB_:
			UI_SUB();
			break;
		case _F_SUB_:
			F_SUB();
			break;
		case _D_SUB_:
			D_SUB();
			break;
		case _L_SUB_:
			L_SUB();
			break;
		case _SB_MUL_:
			SB_MUL();
			break;
		case _UB_MUL_:
			UB_MUL();
			break;
		case _SI_MUL_:
			SI_MUL();
			break;
		case _UI_MUL_:
			UI_MUL();
			break;
		case _F_MUL_:
			F_MUL();
			break;
		case _D_MUL_:
			D_MUL();
			break;
		case _L_MUL_:
			L_MUL();
			break;
		case _SB_DIV_:
			SB_DIV();
			break;
		case _UB_DIV_:
			UB_DIV();
			break;
		case _SI_DIV_:
			SI_DIV();
			break;
		case _UI_DIV_:
			UI_DIV();
			break;
		case _F_DIV_:
			F_DIV();
			break;
		case _D_DIV_:
			D_DIV();
			break;
		case _L_DIV_:
			L_DIV();
			break;
		case _SB_REM_:
			SB_REM();
			break;
		case _UB_REM_:
			UB_REM();
			break;
		case _SI_REM_:
			SI_REM();
			break;
		case _UI_REM_:
			UI_REM();
			break;
		case _F_REM_:
			F_REM();
			break;
		case _D_REM_:
			D_REM();
			break;
		case _L_REM_:
			L_REM();
			break;
		case _SB_STORE_:
			SB_STORE();
			break;
		case _UB_STORE_:
			UB_STORE();
			break;
		case _SI_STORE_:
			SI_STORE();
			break;
		case _UI_STORE_:
			UI_STORE();
			break;
		case _F_STORE_:
			F_STORE();
			break;
		case _D_STORE_:
			D_STORE();
			break;
		case _L_STORE_:
			L_STORE();
			break;
		case _REF_STORE_:
			REF_STORE();
			break;
		case _SB_LOAD_:
			SB_LOAD();
			break;
		case _UB_LOAD_:
			UB_LOAD();
			break;
		case _SI_LOAD_:
			SI_LOAD();
			break;
		case _UI_LOAD_:
			UI_LOAD();
			break;
		case _F_LOAD_:
			F_LOAD();
			break;
		case _D_LOAD_:
			D_LOAD();
			break;
		case _L_LOAD_:
			L_LOAD();
			break;
		case _REF_LOAD_:
			REF_LOAD();
			break;
		case _POP_:
			POP();
			break;
		case _POP2_:
			POP2();
			break;
		case _DUP_:
			DUP();
			break;
		case _DUP2_:
			DUP2();
			break;
		case _GOTO_:
			GOTO();
			break;
		

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
	std::stack<container> temp = m_stack;
	std::stack<container> result;
	std::cout << std::dec;
	while(!temp.empty()){
		result.push(temp.top());
		temp.pop();
	}
	while(!result.empty()){
		switch(result.top().type){
			case _NULL_:
				std::cout <<"NULL\n";
				break;
			case _SBYTE_:
				std::cout << (sint)(signed char)(result.top().data & 0xff) << "\n";
				break;
			case _UBYTE_:
				std::cout << (result.top().data & 0xff) << "\n";
				break;
			case _SINT_:
				std::cout << *reinterpret_cast<sint*>(&result.top().data) << "\n";
				break;
			case _UINT_:
				std::cout << result.top().data << "\n";
				break;
			case _FLOAT_:
				std::cout <<  *reinterpret_cast<float*>(&result.top().data) << "\n";
				break;
			case _DOUBLE_:
				std::cout << *reinterpret_cast<double*>(&result.top().data) << "\n";
				break;
			case _LONG_:
				std::cout << (slong)result.top().data << "\n";
				break;
			case _STRING_:
				std::cout << (slong)result.top().data << "\n";
				break;
			case _REF_:
				std::cout << "0x" << std::hex << result.top().data << std::dec << "\n";
				break;
		}
		
		result.pop();
	}
}

void VM::printVariableTable(){

}