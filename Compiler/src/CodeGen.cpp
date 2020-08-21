#include "CodeGen.h"

void CodeGen::generate(){
	for(current_pos = 0; current_pos < m_table.size(); current_pos++){
		if(m_table[current_pos].op == TAC_FN_START)
			genFunction();
		else if(m_table[current_pos].op == TAC_LABEL)
			genLabel();
		else if (m_table[current_pos].result[0] == '_')
			genTmpVar();
		else if (m_table[current_pos].op == TAC_RET)
			genRet();
		else if (std::isalpha(m_table[current_pos].result[0]) && m_table[current_pos].op != TAC_FN_END)
			genVar();
		DBG_PRINT(m_table[current_pos].op << '\n');
	}
	genConstTable();
	writeFile();
}

void CodeGen::genFunction(){
	DBG_PRINT("Generating function\n");
	push_string(m_table[current_pos-1].result);
	code.push_back('\0');
	code.push_back(0);		//@TODO local variable count
	code.push_back(0);		//@TODO arg count
	//code.push_back(0x02);	//@TODO arg types
	current_pos++;                  // skip begin func quad
	for(; m_table[current_pos].op != TAC_FN_END; current_pos++){
		if(m_table[current_pos].op == TAC_LABEL)
			genLabel();
		else if (m_table[current_pos].result[0] == '_')
			genTmpVar();
		else if (m_table[current_pos].op == 7)
			genRet();
		else if (std::isalpha(m_table[current_pos].result[0]) && m_table[current_pos].op != TAC_FN_END)  // remove second check
			genVar();
		DBG_PRINT(m_table[current_pos].op << '\n');
	}
	DBG_PRINT("End Func\n");
}

void CodeGen::genTmpVar(){          //@TODO support other types. For now int is assumed
	DBG_PRINT("Generating tmp var\n");
	switch(m_table[current_pos].op){
		case TAC_EQUAL:
			push_int64(str_to_int64(m_table[current_pos].operand1));
			break;
		case TAC_ADD:
			code.push_back(_SI_ADD_);
			break;
		case TAC_SUB:
			code.push_back(_SI_SUB_);
			break;
		case TAC_MUL:
			code.push_back(_SI_MUL_);
			break;
		case TAC_DIV:
			code.push_back(_SI_DIV_);
			break;
		case TAC_MOD:
			code.push_back(_SI_REM_);
			break;
	}
	DBG_PRINT(m_table[current_pos].operand1);
}

void CodeGen::genVar(){
	DBG_PRINT("Generating var\n");
}

void CodeGen::genRet(){
	DBG_PRINT("Generating ret\n");
	code.push_back(_RET_);
}

void CodeGen::genLabel(){
	DBG_PRINT("Generating Label\n");
	label_table.push_back(Label{m_table[current_pos].result, code.size()-1, m_table[current_pos+1].op == TAC_FN_START});

}

void CodeGen::genConstTable(){
	unsigned char fn_count = 0;
	const_table.push_back(0x00);
	for(Label lbl : label_table){
		if(lbl.is_fn){
			fn_count++;
			for(unsigned char uc: lbl.label)
				const_table.push_back(uc);
			const_table.push_back('\0');
		}
	}
	const_table[0] = fn_count;
}

void CodeGen::push_string(std::string& str){
	for(unsigned char uc: str)  // hhmmmmm... might change
		code.push_back(uc);
}

void CodeGen::push_int64(int64_t integer){
	code.push_back(_SICONST_PUSH_);
	for(int i = 0; i < 8; i++){
		DBG_PRINT(integer << "|\n");
		code.push_back((unsigned char)(integer & 0xFF));
		integer >>= 8;
	}
}

int64_t CodeGen::str_to_int64(std::string& str){
	std::istringstream ss(str);
	int64_t int64;
	ss >> int64;
	return int64;
}

void CodeGen::writeFile(){
	unsigned int file_size = sizeof(file_signature) + sizeof(bootstrap) + const_table.size() + code.size();
	unsigned char* data = (unsigned char*)malloc(file_size);
	uintptr_t ptr = (uintptr_t)data;
	memcpy(data, file_signature, sizeof(file_signature));
	ptr += sizeof(file_signature);
	memcpy((void*)ptr, const_table.data(), const_table.size());
	ptr += const_table.size();
	memcpy((void*)ptr, bootstrap, sizeof(bootstrap));
	ptr += sizeof(bootstrap);
	memcpy((void*)ptr, code.data(), code.size());
	ptr += code.size();
	std::ofstream file("out.arcb", std::ios::out|std::ios::binary|std::ios::trunc);
	if(file.is_open())
		file.write((char *)data, file_size);
	file.close();
}