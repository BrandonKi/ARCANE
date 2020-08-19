#include "CodeGen.h"

void CodeGen::generate(){
    for(current_pos = 0; current_pos < m_table.size(); current_pos++){
        if(m_table[current_pos].op == 4)
            genFunction();
        else if(m_table[current_pos].op == 3)
            genLabel();
        else if (m_table[current_pos].result[0] == '_')
            genTmpVar();
        else if (m_table[current_pos].op == 7)
            genRet();
        else if (std::isalpha(m_table[current_pos].result[0]) && m_table[current_pos].op != 5)
            genVar();
        DBG_PRINT(m_table[current_pos].op << '\n');
    }
    unsigned int file_size = sizeof(file_signature) + sizeof(bootstrap) + const_table.size() + code.size();
    unsigned char* data = (unsigned char*)malloc(file_size);
    uintptr_t ptr = (uintptr_t)data;
    memcpy(data, file_signature, sizeof(file_signature));
    DBG_PRINT(data);
    ptr += sizeof(file_signature);
    memcpy((void*)ptr, bootstrap, sizeof(bootstrap));
    ptr += sizeof(bootstrap);
    memcpy((void*)ptr, const_table.data(), const_table.size());
    ptr += const_table.size();
    memcpy((void*)ptr, code.data(), code.size());
    ptr += code.size();
    std::ofstream file("out.arcb", std::ios::out|std::ios::binary|std::ios::trunc);
    if(file.is_open())
        file.write((char *)data, file_size);
    file.close();
}

void CodeGen::genFunction(){
    DBG_PRINT("Generating function\n");
    for(unsigned char uc: m_table[current_pos-1].result)  // hhmmmmm... might change
        code.push_back(uc);
    current_pos++;                  // skip begin func quad
    for(; m_table[current_pos].op != 5; current_pos++){
        if(m_table[current_pos].op == 3)
            genLabel();
        else if (m_table[current_pos].result[0] == '_')
            genTmpVar();
        else if (m_table[current_pos].op == 7)
            genRet();
        else if (std::isalpha(m_table[current_pos].result[0]) && m_table[current_pos].op != 5)
            genVar();
        DBG_PRINT(m_table[current_pos].op << '\n');
    }
    DBG_PRINT("End Func\n");
}

void CodeGen::genTmpVar(){
    DBG_PRINT("Generating tmp var\n");
    
}

void CodeGen::genVar(){
    DBG_PRINT("Generating var\n");
}

void CodeGen::genRet(){
    DBG_PRINT("Generating ret\n");
}

void CodeGen::genLabel(){
    DBG_PRINT("Generating Label\n");
    label_table.push_back(Label{m_table[current_pos].result, code.size()-1, m_table[current_pos+1].op == 4});
}