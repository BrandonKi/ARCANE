#include <iostream>
#include <cstring>
#include <string>
#include <stack>
#include <cmath>
#include <vector>

#include "MNEMONICS.h"

#define log(x) std::cout << x
#define logn(x) std::cout << x << "\n"
#define ERR(x) std::cout << x << "\n"; std::cin.get(); return -1

#define INT_SIZE 4         // This macro doesn't do anything 

#ifdef DEBUG_BUILD
    #define NEWLINE std::cout << "\n"
    #define POINTER(x) std::cout << "    POINTER: " << x << "\n"
    #define INSTRUCTION(x) std::cout << "INSTRUCTION: " << x << "\n"
    #define MNEMONIC(x) std::cout << "   MNEMONIC: " << x << "\n"
    #define DEBUG(x) std::cout <<  " DEBUG INFO: " << x << "\n"
    #define WAIT_FOR_INPUT std::cin.get()
    #define DEBUG_STACK printStack()
    #define EXIT_MSG(x) std::cout << "EXITED WITH CODE: " << x << "\n"
#else
    #define NEWLINE
    #define POINTER(x)
    #define INSTRUCTION(x)
    #define MNEMONIC(x)
    #define DEBUG(x)
    #define WAIT_FOR_INPUT
    #define DEBUG_STACK
    #define EXIT_MSG(x)
#endif

typedef uint_fast64_t u64;
typedef int_fast64_t sint;
typedef uint_fast8_t byte;
typedef uint_fast32_t u32;

struct container {
    byte type;
    u64 data;
};

class VM {

private:
    std::vector<std::pair<std::string, int>> functionTable;
    u32 functionTable_len;
    byte* m_data; 
    u64 m_size;
    u64 m_memptr;
    u32 m_lclptr;
    u32 m_frameptr;
    container* m_variableTable;         // c arr
    u32 m_variableTable_len;
    std::vector<container> m_stack;

    bool EXIT_ON_NEXT_INTSRUCTION = false;
    u32 EXIT_CODE;


    int findSymbol(std::string);
    void printStack();
    void printTempStack();
    void printVariableTable();
    byte* getNextByte();
    void nextInstruction();
    void executeInstruction();

public:
    VM(char* data, u64 size);
    ~VM(){delete[] m_data;}
    void printProgram(byte*);
    inline byte* getProgram(){return m_data;}
    void run();
    void freeMem();

private:
    inline void NOP(){
        MNEMONIC("NOP");
    }
    inline void EXIT(){
        MNEMONIC("EXIT");
        EXIT_ON_NEXT_INTSRUCTION = true;
        EXIT_CODE = *getNextByte();
    }
    inline void RET(){                       // IMPLEMENT ME
        MNEMONIC("RET");
        container result = m_stack.back();
        u32 end_frame = m_lclptr;
        m_memptr = m_stack[m_frameptr+1].data;
        m_lclptr = m_stack[m_frameptr+2].data;
        m_frameptr = m_stack[m_frameptr+3].data;
        m_stack.erase(m_stack.begin()+end_frame, m_stack.end());
        m_stack.push_back(result);
    }
    inline void NCONST_PUSH(){
        MNEMONIC("NCONST_PUSH");
        m_stack.push_back({_NULL_, 0});
    }
    inline void SBCONST_PUSH(){
        MNEMONIC("SBCONST_PUSH");
        m_stack.push_back({_SBYTE_, *getNextByte()});
        DEBUG(std::dec << (sint)(signed char)(m_stack.back().data & 0xff));
    }
    inline void UBCONST_PUSH(){
        MNEMONIC("UBCONST_PUSH");
        m_stack.push_back({_UBYTE_, (u64)*getNextByte()});
        DEBUG(std::dec << (u64)m_stack.back().data);
    }
    inline void SICONST_PUSH(){
        MNEMONIC("SICONST_PUSH");
        m_stack.push_back({_SINT_, *reinterpret_cast<u64*>(&m_data[++m_memptr])});
        m_memptr += 7;
        DEBUG(std::dec << (sint)m_stack.back().data);
    }
    inline void UICONST_PUSH(){
        MNEMONIC("UICONST_PUSH");
        m_stack.push_back({_UINT_, *reinterpret_cast<u64*>(&m_data[++m_memptr])});
        m_memptr += 7;
        DEBUG(std::dec << (u64)m_stack.back().data);
    }
    inline void FCONST_PUSH(){
        MNEMONIC("FCONST_PUSH");
        m_stack.push_back({_FLOAT_, *reinterpret_cast<u64*>(&m_data[++m_memptr])});
        m_memptr += 3;
        DEBUG(std::dec << *reinterpret_cast<float*>(&m_stack.back().data));
    }
    inline void DCONST_PUSH(){                          
        MNEMONIC("DCONST_PUSH");
        m_stack.push_back({_DOUBLE_, *reinterpret_cast<u64*>(&m_data[++m_memptr])});
        m_memptr += 7;
        DEBUG(std::dec << (double)m_stack.back().data);
    }
    inline void SCONST_PUSH(){
        MNEMONIC("SCONST_PUSH");                        // FIX THIS
    }
    inline void CALL_LOCAL(){                           // IMPLEMENT ME
        MNEMONIC("CALL_LOCAL");
        u32 index = *(u32*)getNextByte();
        std::string fn_name = functionTable[index].first;
        m_memptr += 3;
        // char* fn_name = (char*)getNextByte();
        DEBUG(fn_name);
        u32 old_memptr = m_memptr;
        u32 old_lclptr = m_lclptr;
        u32 old_frameptr = m_frameptr;
        for(std::pair<std::string, int> pair : functionTable)
            if(pair.first == fn_name)
                m_memptr = pair.second;
        m_lclptr = m_stack.size() - (int)(m_data[m_memptr+1]);
        for(u32 i = 0; i < (int)(m_data[m_memptr+1]) - (int)(m_data[m_memptr]); i++){
            m_stack.push_back(container{_UNDEFINED_, 0});
        }
        m_memptr += (int)(m_data[m_memptr+1]) + 1;
        m_frameptr = m_stack.size();
        m_stack.push_back(container{_FRAME_, 0});
        m_stack.push_back(container{_FRAME_INFO_, old_memptr});
        m_stack.push_back(container{_FRAME_INFO_, old_lclptr});
        m_stack.push_back(container{_FRAME_INFO_, old_frameptr});
        m_stack.push_back(container{_FRAME_, 0});
    }
    inline void ARR_LEN(){
        MNEMONIC("ARR_LEN");
        u64 len = *reinterpret_cast<u64*>(m_stack.back().data);
        DEBUG(len);
        m_stack.pop_back();
        m_stack.push_back({_UINT_,len});
    }
    inline void NEW_ARR(){
        MNEMONIC("NEW_ARR");
        container top = m_stack.back();
        m_stack.pop_back();
        if((top.type == _NULL_ || top.type >= 8)  ||                     // check for invalid type
            (top.type == _SBYTE_ && (sint)(signed char)top.data < 0) ||  // check for negative SBYTE
                (top.type == _SINT_ && (sint)top.data < 0)){             // check for negative SINT
            logn("INVALID_ARRAY_LENGTH");
            std::exit(-1);
        }
        u64 length = top.data;
        void* temp;
        switch (*getNextByte()){
            case _NULL_:
                // do nothing
                break;
            case _SBYTE_:
                temp = (signed char*)malloc(length + 8);            // leave the first 64 bits for the length
                *(u64*)temp = length;                              // set first 64 bits equal to length
                m_stack.push_back({_REF_, (u64)(signed char*)temp});
                break;
            case _UBYTE_:                                                               
                temp = (unsigned char*)malloc(length + 8);
                *(u64*)temp = length;
                m_stack.push_back({_REF_, reinterpret_cast<u64>(temp)});
                break;
            case _SINT_:
                temp = (sint*)malloc(8 * length + 8);
                *(u64*)temp = length;
                m_stack.push_back({_REF_, (u64)(sint*)temp});
                break;
            case _UINT_:
                temp = (u64*)malloc(8 * length + 8);
                *(u64*)temp = length;
                m_stack.push_back({_REF_, (u64)(u64*)temp});
                break;
            case _FLOAT_:
                temp = (float*)malloc(4 * length + 8);
                *(u64*)temp = length;
                m_stack.push_back({_REF_, (u64)(float*)temp});
                break;
            case _DOUBLE_:
                temp = (double*)malloc(8 * length + 8);
                *(u64*)temp = length;
                m_stack.push_back({_REF_, (u64)(double*)temp});
                break;
            case _STRING_:
                
                break;
            case _REF_:
                
                break;
            
            default:
                break;
        }
        DEBUG("length: " << *reinterpret_cast<u64*>(temp) << " ref: 0x" << std::hex << m_stack.back().data << std::dec);
    }
    inline void UBA_STORE(){
        MNEMONIC("UBA_STORE");
        u64 index = m_stack.back().data & 0xff;
        m_stack.pop_back();
        u64 value = m_stack.back().data & 0xff;
        m_stack.pop_back();
        unsigned char* arr = reinterpret_cast<unsigned char*>(m_stack.back().data);
        m_stack.pop_back();
        if(index > *reinterpret_cast<uint_fast32_t*>(arr)){
            EXIT_CODE = 1;
            EXIT_ON_NEXT_INTSRUCTION = true;
            logn("ARRAY_OUT_OF_BOUNDS");
        }else{
            *(arr + 4 + index) = value;
            DEBUG("index: " << index << " value: " << " " << (u64)*(arr + 4 + index));
        }
    }
    inline void UBA_LOAD(){
        MNEMONIC("UBA_LOAD");
        u64 index = m_stack.back().data & 0xff;
        m_stack.pop_back();
        unsigned char* arr = reinterpret_cast<unsigned char*>(m_stack.back().data);
        m_stack.pop_back();
        if(index > *reinterpret_cast<uint_fast32_t*>(arr)){
            EXIT_CODE = 1;
            EXIT_ON_NEXT_INTSRUCTION = true;
            logn("ARRAY_OUT_OF_BOUNDS");
        }else{
            m_stack.push_back({_UBYTE_, *(arr + 4 + index)});
            DEBUG("index: " << index << " value: " << *(arr + 4 + index));
        }
    }
    inline void SB_ADD(){
        MNEMONIC("SB_ADD");
        sint result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result += (m_stack.back().data & 0xff);
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SBYTE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UB_ADD(){
        MNEMONIC("UB_ADD");
        u64 result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result += (m_stack.back().data & 0xff);
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UBYTE_, result});
    }
    inline void SI_ADD(){
        MNEMONIC("SI_ADD");
        sint result = m_stack.back().data;
        m_stack.pop_back();
        result += m_stack.back().data;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SINT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UI_ADD(){
        MNEMONIC("UI_ADD");
        u64 result = m_stack.back().data;
        m_stack.pop_back();
        result += m_stack.back().data;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UINT_, result});
    }
    inline void F_ADD(){
        MNEMONIC("F_ADD");
        float result = *reinterpret_cast<float*>(&m_stack.back().data);
        m_stack.pop_back();
        result += *reinterpret_cast<float*>(&m_stack.back().data);
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_FLOAT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void D_ADD(){
        MNEMONIC("D_ADD");
        double result = m_stack.back().data;
        m_stack.pop_back();
        result += *reinterpret_cast<double*>(&m_stack.back().data);
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_DOUBLE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void SB_SUB(){
        MNEMONIC("SB_SUB");
        sint result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result = (m_stack.back().data & 0xff) - result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SBYTE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UB_SUB(){
        MNEMONIC("UB_SUB");
        u64 result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result = (m_stack.back().data - result) & 0xff;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UBYTE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void SI_SUB(){
        MNEMONIC("SI_SUB");
        sint result = m_stack.back().data;
        m_stack.pop_back();
        result = *reinterpret_cast<sint*>(&m_stack.back().data) - result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SINT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UI_SUB(){
        MNEMONIC("UI_SUB");
        u64 result = m_stack.back().data;
        m_stack.pop_back();
        result = m_stack.back().data - result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UINT_, result});
    }
    inline void F_SUB(){
        MNEMONIC("F_SUB");
        float result = *reinterpret_cast<float*>(&m_stack.back().data);
        m_stack.pop_back();
        result = *reinterpret_cast<float*>(&m_stack.back().data) - result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_FLOAT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void D_SUB(){
        MNEMONIC("D_SUB");
        double result = m_stack.back().data;
        m_stack.pop_back();
        result = *reinterpret_cast<double*>(&m_stack.back().data) - result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_DOUBLE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void SB_MUL(){
        MNEMONIC("SB_MUL");
        sint result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result = (m_stack.back().data & 0xff) * result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SBYTE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UB_MUL(){
        MNEMONIC("UB_MUL");
        u64 result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result = (m_stack.back().data * result) & 0xff;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UBYTE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void SI_MUL(){
        MNEMONIC("SI_MUL");
        sint result = m_stack.back().data;
        m_stack.pop_back();
        result = m_stack.back().data * result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SINT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UI_MUL(){
        MNEMONIC("UI_MUL");
        u64 result = m_stack.back().data;
        m_stack.pop_back();
        result = m_stack.back().data * result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UINT_, result});
    }
    inline void F_MUL(){
        MNEMONIC("F_SUB");
        float result = *reinterpret_cast<float*>(&m_stack.back().data);
        m_stack.pop_back();
        result = *reinterpret_cast<float*>(&m_stack.back().data) * result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_FLOAT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void D_MUL(){
        MNEMONIC("D_MUL");
        double result = *reinterpret_cast<double*>(&m_stack.back().data);
        m_stack.pop_back();
        result = *reinterpret_cast<double*>(&m_stack.back().data) * result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_DOUBLE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void SB_DIV(){
        MNEMONIC("SB_DIV");
        sint result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result = (m_stack.back().data & 0xff) / result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SBYTE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UB_DIV(){
        MNEMONIC("UB_DIV");
        u64 result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result = (m_stack.back().data / result) & 0xff;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UBYTE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void SI_DIV(){
        MNEMONIC("SI_DIV");
        sint result = m_stack.back().data;
        m_stack.pop_back();
        result = m_stack.back().data / result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SINT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UI_DIV(){
        MNEMONIC("UI_DIV");
        u64 result = m_stack.back().data;
        m_stack.pop_back();
        result = m_stack.back().data / result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UINT_, result});
    }
    inline void F_DIV(){
        MNEMONIC("F_DIV");
        float result = *reinterpret_cast<float*>(&m_stack.back().data);
        m_stack.pop_back();
        result = *reinterpret_cast<float*>(&m_stack.back().data) / result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_FLOAT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void D_DIV(){
        MNEMONIC("D_DIV");
        double result = *reinterpret_cast<double*>(&m_stack.back().data);
        m_stack.pop_back();
        result = *reinterpret_cast<double*>(&m_stack.back().data) / result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_DOUBLE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void SB_REM(){
        MNEMONIC("SB_DIV");
        sint result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result = (m_stack.back().data & 0xff) % result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SBYTE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UB_REM(){
        MNEMONIC("UB_DIV");
        u64 result = (m_stack.back().data & 0xff);
        m_stack.pop_back();
        result = (m_stack.back().data % result) & 0xff;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UBYTE_, *reinterpret_cast<u64*>(&result)});
    }
    inline void SI_REM(){
        MNEMONIC("SI_DIV");
        sint result = m_stack.back().data;
        m_stack.pop_back();
        result = m_stack.back().data % result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_SINT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void UI_REM(){
        MNEMONIC("UI_DIV");
        u64 result = m_stack.back().data;
        m_stack.pop_back();
        result = m_stack.back().data % result;
        m_stack.pop_back();
        DEBUG(std::dec << result);
        m_stack.push_back({_UINT_, result});
    }
    inline void F_REM(){
        MNEMONIC("F_DIV");
        float temp = *reinterpret_cast<float*>(&m_stack.back().data);
        m_stack.pop_back();
        float result = std::modf(*reinterpret_cast<float*>(&m_stack.back().data), &temp);
        m_stack.pop_back();
        DEBUG(std::dec << (float)result);
        m_stack.push_back({_FLOAT_, *reinterpret_cast<u64*>(&result)});
    }
    inline void D_REM(){
        MNEMONIC("D_REM");
        double temp = *reinterpret_cast<double*>(&m_stack.back().data);
        m_stack.pop_back();
        double result = std::modf(*reinterpret_cast<double*>(&m_stack.back().data), &temp);
        m_stack.pop_back();
        DEBUG(std::dec << (double)result);
        m_stack.push_back({_DOUBLE_, *reinterpret_cast<u64*>(&result)});
    }

    inline void SB_STORE(){
        MNEMONIC("SB_STORE");
        u64 index = *getNextByte();
        m_stack[m_lclptr + index] = {_SBYTE_, (*reinterpret_cast<u64*>(&m_stack.back().data) & 0xff)};               // take the 8 least significant bits by using bitmask 0xff and & to convert to byte
        DEBUG("slot: " << index << " value: " << (sint)(int_fast8_t)(m_stack[m_lclptr + index].data & 0xff));           // I could just do % BYTE_MAX_VALUE but no
        m_stack.pop_back();
    }
    inline void UB_STORE(){
        MNEMONIC("UB_STORE");
        u64 index = *getNextByte();
        m_stack[m_lclptr + index] = {_UBYTE_, (m_stack.back().data & 0xff)};                     // take the 8 least significant bits by using bitmask 0xff and & to convert to byte
        DEBUG("slot: " << index << " value: " << m_stack[m_lclptr + index].data);              // I could just do % BYTE_MAX_VALUE but no
        m_stack.pop_back();
    }
    inline void SI_STORE(){
        MNEMONIC("SI_STORE");
        u64 index = *getNextByte();
        m_stack[m_lclptr + index] = m_stack.back();
        DEBUG("slot: " << index << " value: " << (sint)(m_stack[m_lclptr + index].data));       
        m_stack.pop_back();
    }
    inline void UI_STORE(){
        MNEMONIC("UI_STORE");
        u64 index = *getNextByte();
        m_stack[m_lclptr + index] = m_stack.back();
        DEBUG("slot: " << index << " value: " << m_stack[m_lclptr + index].data);       
        m_stack.pop_back();
    }
    inline void F_STORE(){
        MNEMONIC("F_STORE");
        u64 index = *getNextByte();
        m_stack[m_lclptr + index] = m_stack.back();
        DEBUG("slot: " << index << " value: " << *reinterpret_cast<float*>(&m_stack[m_lclptr + index].data));
        m_stack.pop_back();
    }
    inline void D_STORE(){
        MNEMONIC("D_STORE");
        u64 index = *getNextByte();
        m_stack[m_lclptr + index] = m_stack.back();
        DEBUG("slot: " << index << " value: " << m_stack[m_lclptr + index].data);       
        m_stack.pop_back();
    }
    inline void REF_STORE(){
        MNEMONIC("REF_STORE");
        u64 index = *getNextByte();
        m_stack[m_lclptr + index] = m_stack.back();
        DEBUG("slot: " << index << " ref: 0x" << std::hex << m_stack[m_lclptr + index].data << std::dec);
        m_stack.pop_back();
    }
    inline void SB_LOAD(){
        MNEMONIC("SB_LOAD");
        u64 index = *getNextByte();
        m_stack.push_back({_SBYTE_, m_stack[m_lclptr + index].data & 0xff});
        DEBUG("slot: " << index << " value: " << (m_stack.back().data & 0xff));
    }
    inline void UB_LOAD(){
        MNEMONIC("UB_LOAD");
        u64 index = *getNextByte();
        m_stack.push_back({_UBYTE_, m_stack[m_lclptr + index].data & 0xff});
        DEBUG("slot: " << index << " value: " << (m_stack.back().data & 0xff));
    }
    inline void SI_LOAD(){
        MNEMONIC("SI_LOAD");
        u64 index = *getNextByte();
        m_stack.push_back(m_stack[m_lclptr + index]);
        DEBUG("slot: " << index << " value: " << (m_stack.back().data & 0xff));
    }
    inline void UI_LOAD(){
        MNEMONIC("UI_LOAD");
        u64 index = *getNextByte();
        m_stack.push_back(m_stack[m_lclptr + index]);
        DEBUG("slot: " << index << " value: " << (m_stack.back().data & 0xff));
    }
    inline void F_LOAD(){
        MNEMONIC("F_LOAD");
        u64 index = *getNextByte();
        m_stack.push_back(m_stack[m_lclptr + index]);
        DEBUG("slot: " << index << " value: " << (m_stack.back().data & 0xff));
    }
    inline void D_LOAD(){
        MNEMONIC("D_LOAD");
        u64 index = *getNextByte();
        m_stack.push_back(m_stack[m_lclptr + index]);
        DEBUG("slot: " << index << " value: " << *reinterpret_cast<double*>(&m_stack.back().data));
    }
    inline void REF_LOAD(){
        MNEMONIC("REF_LOAD");
        u64 index = *getNextByte();
        m_stack.push_back(m_stack[m_lclptr + index]);
        DEBUG("slot: " << index << " ref: 0x" << std::hex << m_stack.back().data << std::dec);
    }
    inline void POP(){
        MNEMONIC("POP");
        m_stack.pop_back();
    }
    inline void POP2(){
        MNEMONIC("POP2");
        m_stack.pop_back();
        m_stack.pop_back();
    }
    inline void DUP(){
        MNEMONIC("DUP");
        m_stack.push_back(m_stack.back());
    }
    inline void DUP2(){
        MNEMONIC("DUP2");
        container temp = m_stack.back();
        m_stack.pop_back();
        container temp2 = m_stack.back();
        m_stack.push_back(temp);
        m_stack.push_back(temp2);
        m_stack.push_back(temp);
    }
    inline void DUP2X(){
        MNEMONIC("DUP2X");
        m_stack.push_back(m_stack.back());
        m_stack.push_back(m_stack.back());
    }
    inline void SWAP(){
        MNEMONIC("SWAP");
        container a = m_stack.back();
        m_stack.pop_back();
        std::swap(a, m_stack.back());
        m_stack.push_back(a);
    }
    inline void JMP_HELPER(byte address){
        if(address == 0)
            m_memptr = 0xffffffffffffffff;
        else
            m_memptr = address-1;       // ugly workaround... haha get it workaround... like integer wraparound...
    }
    inline void JMP(){
        byte address = *getNextByte();
        MNEMONIC("JMP");
        DEBUG(std::hex << (u64)address << std::dec);
        JMP_HELPER(address);
    }
    inline void JIFNE(){
        u64 temp = m_stack.back().data;
        byte address = *getNextByte();
        m_stack.pop_back();
        MNEMONIC("JIFNE");
        DEBUG(temp << " != " << m_stack.back().data << " " << std::hex << (u64)address << std::dec);
        if(temp != m_stack.back().data)
            JMP_HELPER(address);
        m_stack.pop_back();
    }
    inline void JIFE(){
        u64 temp = m_stack.back().data;
        byte address = *getNextByte();
        m_stack.pop_back();
        MNEMONIC("JIFE");
        DEBUG(temp << " == " << m_stack.back().data << " " << std::hex << (u64)address << std::dec);
        if(temp == m_stack.back().data)
            JMP_HELPER(address);
        m_stack.pop_back();
    }
    inline void JIFLS(){
        u64 temp = m_stack.back().data;
        byte address = *getNextByte();
        m_stack.pop_back();
        MNEMONIC("JIFLS");
        DEBUG(temp << " < " << m_stack.back().data << " " << std::hex << (u64)address << std::dec);
        if(temp < m_stack.back().data)
            JMP_HELPER(address);
        m_stack.pop_back();
    }
    inline void JIFGT(){
        u64 temp = m_stack.back().data;
        byte address = *getNextByte();
        m_stack.pop_back();
        MNEMONIC("JIFGT");
        DEBUG(temp << " > " << m_stack.back().data << " " << std::hex << (u64)address << std::dec);
        if(temp > m_stack.back().data)
            JMP_HELPER(address);
        m_stack.pop_back();
    }
    inline void JIFZ(){
        byte address = *getNextByte();
        MNEMONIC("JIFZ");
        DEBUG(m_stack.back().data << " == 0 " << std::hex << (u64)address << std::dec);
        if(m_stack.back().data == 0)
            JMP_HELPER(address);
        m_stack.pop_back();
    }
    inline void JIFNZ(){
        byte address = *getNextByte();
        MNEMONIC("JIFNZ");
        DEBUG(m_stack.back().data << " != 0 " << std::hex << (u64)address << std::dec);
        if(m_stack.back().data != 0)
            JMP_HELPER(address);
        m_stack.pop_back();
    }
};