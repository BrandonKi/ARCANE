#include "CodeGen.h"

void CodeGen::generate(){
	for(current_pos = 0; current_pos < m_table.size(); current_pos++){
		if(m_table[current_pos].op == TAC_FN_START)
			genFunction();
		else if(m_table[current_pos].op == TAC_LABEL)
			genLabel();
		else if (m_table[current_pos].result.data[0] == '_')
			genTmpVar();
		else if (m_table[current_pos].op == TAC_RET)
			genRet();
		else if (std::isalpha(m_table[current_pos].result.data[0]) && m_table[current_pos].op != TAC_FN_END)
			genVar();
	}
	genConstTable();
	writeFile();
}

void CodeGen::genFunction(){
	DBG_PRINT("Generating function\n");
	push_string(m_table[current_pos-1].result.data);
	code.push_back('\0');
	code.push_back(0);		//@TODO local variable count
	local_var_index = code.size()-1;
	code.push_back(0);		//@TODO arg count
	//code.push_back(0x02);	//@TODO arg types
	current_pos++;                  // skip begin func quad/node
	for(; m_table[current_pos].op != TAC_FN_END; current_pos++){
		if(m_table[current_pos].op == TAC_LABEL)
			genLabel();
		else if (m_table[current_pos].result.data[0] == '_')
			genTmpVar();
		else if (m_table[current_pos].op == 7)
			genRet();
		else if (std::isalpha(m_table[current_pos].result.data[0]))
			genVar();
		else
			logn("FALLTHROUGH" << m_table[current_pos].operand1.type);
		DBG_PRINT(m_table[current_pos].op << '\n');
	}
	DBG_PRINT("End Func\n");
}

void CodeGen::genTmpVar(){          //@TODO support other types. For now int is assumed
	DBG_PRINT("Generating tmp var\n");
	if(std::isalpha(m_table[current_pos].operand1.data[0]) || std::isalpha(m_table[current_pos].operand2.data[0]))
		if(std::isalpha(m_table[current_pos].operand1.data[0])){
			loadVar(m_table[current_pos].operand1.data);
		}
		else if(std::isalpha(m_table[current_pos].operand2.data[0])){
			loadVar(m_table[current_pos].operand2.data);
		}
		else{
			loadVar(m_table[current_pos].operand1.data);
			loadVar(m_table[current_pos].operand2.data);
		}
	switch(m_table[current_pos].result.type){
		case ST_INT:
			switch(m_table[current_pos].op){
				case TAC_EQUAL:
					if(std::isalpha(m_table[current_pos].operand1.data[0])){
						loadVar(m_table[current_pos].operand1.data);
					}
					push_int64(str_to_int64(m_table[current_pos].operand1.data));
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
		break;
		
	}
}

void CodeGen::genVar(){
	DBG_PRINT("Generating var\n");
	m_lcl_var_table.push_back({m_table[current_pos].result.data, m_table[current_pos].result.type});
	code[local_var_index] = code[local_var_index] + 1;
	code.push_back(_SI_STORE_);
	unsigned char temp = m_lcl_var_table.size() & 0xff;
	code.push_back(temp);
	// push_int64((int64_t)m_lcl_var_table.size());
}

void CodeGen::loadVar(std::string& var){
	logn("LOADING VAR");
	for(unsigned int i = 0; i < m_lcl_var_table.size(); i++){
		if(var == m_lcl_var_table[i].data){
			logn(m_lcl_var_table[i].type);
			switch(m_lcl_var_table[i].type){
				case ST_INT:
					logn("ST_INT");
					code.push_back(_SI_LOAD_);
				break;

			}
		}
	}
}

void CodeGen::genRet(){
	DBG_PRINT("Generating ret\n");
	code.push_back(_RET_);
}

void CodeGen::genLabel(){
	DBG_PRINT("Generating Label\n");
	label_table.push_back(Label{m_table[current_pos].result.data, static_cast<unsigned int>(code.size()-1), m_table[current_pos+1].op == TAC_FN_START});

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
	std::ofstream file("a.arcb", std::ios::out|std::ios::binary|std::ios::trunc);
	if(file.is_open())
		file.write((char *)data, file_size);
	file.close();
}