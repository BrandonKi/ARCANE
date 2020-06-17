#include <iostream>
#include <cstring>
#include <stack>
#include <cmath>

#include "../MNEMONICS.h"

#define DEBUG_BUILD
// #define RELEASE_BUILD

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

typedef uint_fast64_t uint;
typedef int_fast64_t sint;
typedef uint_fast64_t slong;  // wrong, incorrect, fix asap
typedef uint_fast8_t byte;

struct container {
    byte type;
    uint data;
};

class VM {

private:
    byte* m_data;
    uint m_size;
    uint m_memptr;
    container* m_variableTable;
    std::stack<container> m_stack;

    bool EXIT_ON_NEXT_INTSRUCTION = false;
    uint EXIT_CODE;

    byte* convertToByteArray(char*);
    void printStack();
    void printTempStack();
    void printVariableTable();
    byte* getNextByte();
    // uint getNextFourBytes();
    void nextInstruction();
    void executeInstruction();

public:
    VM(char* data, uint size);
    ~VM(){delete[] m_data;};
    void printProgram(byte*);
    inline byte* getProgram(){return m_data;};
    void run();

private:
    inline void NOP(){
        MNEMONIC("NOP");
    }
    inline void EXIT(){
        MNEMONIC("EXIT");
        EXIT_ON_NEXT_INTSRUCTION = true;
        EXIT_CODE = *getNextByte();
    }
    inline void NCONST_PUSH(){
        MNEMONIC("NCONST_PUSH");
        m_stack.push({_NULL_, 0});
    }
    inline void SBCONST_PUSH(){
        MNEMONIC("SBCONST_PUSH");
        m_stack.push({_SBYTE_, *getNextByte()});
        DEBUG(std::dec << (sint)(signed char)(m_stack.top().data & 0xff));
    }
    inline void UBCONST_PUSH(){
        MNEMONIC("UBCONST_PUSH");
        m_stack.push({_UBYTE_, (uint)*getNextByte()});
        DEBUG(std::dec << (uint)m_stack.top().data);
    }
    inline void SICONST_PUSH(){
        MNEMONIC("SICONST_PUSH");
        m_stack.push({_SINT_, *reinterpret_cast<uint*>(&m_data[++m_memptr])});
        m_memptr += 7;
        DEBUG(std::dec << (sint)m_stack.top().data);
    }
    inline void UICONST_PUSH(){
        MNEMONIC("UICONST_PUSH");
        m_stack.push({_UINT_, *reinterpret_cast<uint*>(&m_data[++m_memptr])});
        m_memptr += 7;
        DEBUG(std::dec << (uint)m_stack.top().data);
    }
    inline void FCONST_PUSH(){
        MNEMONIC("FCONST_PUSH");
        m_stack.push({_FLOAT_, *reinterpret_cast<uint*>(&m_data[++m_memptr])});
        m_memptr += 3;
        DEBUG(std::dec << *reinterpret_cast<float*>(&m_stack.top().data));
    }
    inline void DCONST_PUSH(){                          
        MNEMONIC("DCONST_PUSH");
        m_stack.push({_DOUBLE_, *reinterpret_cast<uint*>(&m_data[++m_memptr])});
        m_memptr += 7;
        DEBUG(std::dec << (double)m_stack.top().data);
    }
    inline void LCONST_PUSH(){                          // FIX THIS
        MNEMONIC("LCONST_PUSH");
    }
    inline void SCONST_PUSH(){
        MNEMONIC("SCONST_PUSH");                        // FIX THIS
    }
    inline void ARR_LEN(){
        MNEMONIC("ARR_LEN");
        uint len = *reinterpret_cast<uint*>(m_stack.top().data);
        DEBUG(len);
        m_stack.pop();
        m_stack.push({_UINT_,len});
    }
    inline void NEW_ARR(){
        MNEMONIC("NEW_ARR");
        container top = m_stack.top();
        m_stack.pop();
        if((top.type == _NULL_ || top.type >= 8)  ||                     // check for invalid type
            (top.type == _SBYTE_ && (sint)(signed char)top.data < 0) ||  // check for negative SBYTE
                (top.type == _SINT_ && (sint)top.data < 0)){             // check for negative SINT
            logn("INVALID_ARRAY_LENGTH");
            std::exit(-1);
        }
        uint length = top.data;
        void* temp;
        switch (*getNextByte()){
            case _NULL_:
                // do nothing
                break;
            case _SBYTE_:
                temp = (signed char*)malloc(length + 8);            // leave the first 64 bits for the length
                *(uint*)temp = length;                              // set first 64 bits equal to length
                m_stack.push({_REF_, (uint)(signed char*)temp});
                break;
            case _UBYTE_:                                                               
                temp = (unsigned char*)malloc(length + 8);
                *(uint*)temp = length;
                m_stack.push({_REF_, reinterpret_cast<uint>(temp)});
                break;
            case _SINT_:
                temp = (sint*)malloc(8 * length + 8);
                *(uint*)temp = length;
                m_stack.push({_REF_, (uint)(sint*)temp});
                break;
            case _UINT_:
                temp = (uint*)malloc(8 * length + 8);
                *(uint*)temp = length;
                m_stack.push({_REF_, (uint)(uint*)temp});
                break;
            case _FLOAT_:
                temp = (float*)malloc(4 * length + 8);
                *(uint*)temp = length;
                m_stack.push({_REF_, (uint)(float*)temp});
                break;
            case _DOUBLE_:
                temp = (double*)malloc(8 * length + 8);
                *(uint*)temp = length;
                m_stack.push({_REF_, (uint)(double*)temp});
                break;
            case _LONG_:
                
                break;
            case _STRING_:
                
                break;
            case _REF_:
                
                break;
            case _SPECIAL_:
                
                break;
            
            default:
                break;
        }
        DEBUG("length: " << *reinterpret_cast<uint*>(temp) << " ref: 0x" << std::hex << m_stack.top().data << std::dec);
    }
    inline void UBA_STORE(){
        MNEMONIC("UBA_STORE");
        uint index = m_stack.top().data & 0xff;
        m_stack.pop();
        uint value = m_stack.top().data & 0xff;
        m_stack.pop();
        unsigned char* arr = reinterpret_cast<unsigned char*>(m_stack.top().data);
        m_stack.pop();
        if(index > *reinterpret_cast<uint_fast32_t*>(arr)){
            EXIT_CODE = 1;
            EXIT_ON_NEXT_INTSRUCTION = true;
            logn("ARRAY_OUT_OF_BOUNDS");
        }else{
            *(arr + 4 + index) = value;
            DEBUG("index: " << index << " value: " << " " << (uint)*(arr + 4 + index));
        }
    }
    inline void UBA_LOAD(){
        MNEMONIC("UBA_LOAD");
        uint index = m_stack.top().data & 0xff;
        m_stack.pop();
        unsigned char* arr = reinterpret_cast<unsigned char*>(m_stack.top().data);
        m_stack.pop();
        if(index > *reinterpret_cast<uint_fast32_t*>(arr)){
            EXIT_CODE = 1;
            EXIT_ON_NEXT_INTSRUCTION = true;
            logn("ARRAY_OUT_OF_BOUNDS");
        }else{
            m_stack.push({_UBYTE_, *(arr + 4 + index)});
            DEBUG("index: " << index << " value: " << *(arr + 4 + index));
        }
    }
    inline void SB_ADD(){
        MNEMONIC("SB_ADD");
        sint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result += (m_stack.top().data & 0xff);
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SBYTE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UB_ADD(){
        MNEMONIC("UB_ADD");
        uint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result += (m_stack.top().data & 0xff);
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UBYTE_, result});
    }
    inline void SI_ADD(){
        MNEMONIC("SI_ADD");
        sint result = m_stack.top().data;
        m_stack.pop();
        result += m_stack.top().data;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SINT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UI_ADD(){
        MNEMONIC("UI_ADD");
        uint result = m_stack.top().data;
        m_stack.pop();
        result += m_stack.top().data;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UINT_, result});
    }
    inline void F_ADD(){
        MNEMONIC("F_ADD");
        float result = *reinterpret_cast<float*>(&m_stack.top().data);
        m_stack.pop();
        result += *reinterpret_cast<float*>(&m_stack.top().data);
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_FLOAT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void D_ADD(){
        MNEMONIC("D_ADD");
        double result = m_stack.top().data;
        m_stack.pop();
        result += *reinterpret_cast<double*>(&m_stack.top().data);
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_DOUBLE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void L_ADD(){                      // FIX THIS
        MNEMONIC("L_ADD");
    }
    inline void SB_SUB(){
        MNEMONIC("SB_SUB");
        sint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result = (m_stack.top().data & 0xff) - result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SBYTE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UB_SUB(){
        MNEMONIC("UB_SUB");
        uint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result = (m_stack.top().data - result) & 0xff;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UBYTE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void SI_SUB(){
        MNEMONIC("SI_SUB");
        sint result = m_stack.top().data;
        m_stack.pop();
        result = *reinterpret_cast<sint*>(&m_stack.top().data) - result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SINT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UI_SUB(){
        MNEMONIC("UI_SUB");
        uint result = m_stack.top().data;
        m_stack.pop();
        result = m_stack.top().data - result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UINT_, result});
    }
    inline void F_SUB(){
        MNEMONIC("F_SUB");
        float result = *reinterpret_cast<float*>(&m_stack.top().data);
        m_stack.pop();
        result = *reinterpret_cast<float*>(&m_stack.top().data) - result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_FLOAT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void D_SUB(){
        MNEMONIC("D_SUB");
        double result = m_stack.top().data;
        m_stack.pop();
        result = *reinterpret_cast<double*>(&m_stack.top().data) - result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_DOUBLE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void L_SUB(){
        MNEMONIC("L_SUB");                         // FIX THIS
    }
    inline void SB_MUL(){
        MNEMONIC("SB_MUL");
        sint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result = (m_stack.top().data & 0xff) * result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SBYTE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UB_MUL(){
        MNEMONIC("UB_MUL");
        uint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result = (m_stack.top().data * result) & 0xff;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UBYTE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void SI_MUL(){
        MNEMONIC("SI_MUL");
        sint result = m_stack.top().data;
        m_stack.pop();
        result = m_stack.top().data * result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SINT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UI_MUL(){
        MNEMONIC("UI_MUL");
        uint result = m_stack.top().data;
        m_stack.pop();
        result = m_stack.top().data * result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UINT_, result});
    }
    inline void F_MUL(){
        MNEMONIC("F_SUB");
        float result = *reinterpret_cast<float*>(&m_stack.top().data);
        m_stack.pop();
        result = *reinterpret_cast<float*>(&m_stack.top().data) * result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_FLOAT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void D_MUL(){
        MNEMONIC("D_MUL");
        double result = *reinterpret_cast<double*>(&m_stack.top().data);
        m_stack.pop();
        result = *reinterpret_cast<double*>(&m_stack.top().data) * result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_DOUBLE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void L_MUL(){
        MNEMONIC("L_MUL");                  // FIX THIS
    }
    inline void SB_DIV(){
        MNEMONIC("SB_DIV");
        sint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result = (m_stack.top().data & 0xff) / result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SBYTE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UB_DIV(){
        MNEMONIC("UB_DIV");
        uint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result = (m_stack.top().data / result) & 0xff;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UBYTE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void SI_DIV(){
        MNEMONIC("SI_DIV");
        sint result = m_stack.top().data;
        m_stack.pop();
        result = m_stack.top().data / result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SINT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UI_DIV(){
        MNEMONIC("UI_DIV");
        uint result = m_stack.top().data;
        m_stack.pop();
        result = m_stack.top().data / result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UINT_, result});
    }
    inline void F_DIV(){
        MNEMONIC("F_DIV");
        float result = *reinterpret_cast<float*>(&m_stack.top().data);
        m_stack.pop();
        result = *reinterpret_cast<float*>(&m_stack.top().data) / result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_FLOAT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void D_DIV(){
        MNEMONIC("D_DIV");
        double result = *reinterpret_cast<double*>(&m_stack.top().data);
        m_stack.pop();
        result = *reinterpret_cast<double*>(&m_stack.top().data) / result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_DOUBLE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void L_DIV(){
        MNEMONIC("L_DIV");              // FIX THIS
    }
    inline void SB_REM(){
        MNEMONIC("SB_DIV");
        sint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result = (m_stack.top().data & 0xff) % result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SBYTE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UB_REM(){
        MNEMONIC("UB_DIV");
        uint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result = (m_stack.top().data % result) & 0xff;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UBYTE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void SI_REM(){
        MNEMONIC("SI_DIV");
        sint result = m_stack.top().data;
        m_stack.pop();
        result = m_stack.top().data % result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SINT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void UI_REM(){
        MNEMONIC("UI_DIV");
        uint result = m_stack.top().data;
        m_stack.pop();
        result = m_stack.top().data % result;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_UINT_, result});
    }
    inline void F_REM(){
        MNEMONIC("F_DIV");
        float temp = *reinterpret_cast<float*>(&m_stack.top().data);
        m_stack.pop();
        float result = std::modf(*reinterpret_cast<float*>(&m_stack.top().data), &temp);
        m_stack.pop();
        DEBUG(std::dec << (float)result);
        m_stack.push({_FLOAT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void D_REM(){
        MNEMONIC("D_REM");
        double temp = *reinterpret_cast<double*>(&m_stack.top().data);
        m_stack.pop();
        double result = std::modf(*reinterpret_cast<double*>(&m_stack.top().data), &temp);
        m_stack.pop();
        DEBUG(std::dec << (double)result);
        m_stack.push({_DOUBLE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void L_REM(){
        MNEMONIC("L_REM");              // FIX THIS
    }
    inline void SB_STORE(){
        MNEMONIC("SB_STORE");
        uint index = *getNextByte();
        m_variableTable[index] = {_SBYTE_, (*reinterpret_cast<uint*>(&m_stack.top().data) & 0xff)};               // take the 8 least significant bits by using bitmask 0xff and & to convert to byte
        DEBUG("slot: " << index << " value: " << (sint)(int_fast8_t)(m_variableTable[index].data & 0xff));           // I could just do % BYTE_MAX_VALUE but no
        m_stack.pop();
    }
    inline void UB_STORE(){
        MNEMONIC("UB_STORE");
        uint index = *getNextByte();
        m_variableTable[index] = {_UBYTE_, (m_stack.top().data & 0xff)};                     // take the 8 least significant bits by using bitmask 0xff and & to convert to byte
        DEBUG("slot: " << index << " value: " << m_variableTable[index].data);              // I could just do % BYTE_MAX_VALUE but no
        m_stack.pop();
    }
    inline void SI_STORE(){
        MNEMONIC("SI_STORE");
        uint index = *getNextByte();
        m_variableTable[index] = m_stack.top();
        DEBUG("slot: " << index << " value: " << (sint)(m_variableTable[index].data));       
        m_stack.pop();
    }
    inline void UI_STORE(){
        MNEMONIC("UI_STORE");
        uint index = *getNextByte();
        m_variableTable[index] = m_stack.top();
        DEBUG("slot: " << index << " value: " << m_variableTable[index].data);       
        m_stack.pop();
    }
    inline void F_STORE(){
        MNEMONIC("F_STORE");
        uint index = *getNextByte();
        m_variableTable[index] = m_stack.top();
        DEBUG("slot: " << index << " value: " << *reinterpret_cast<float*>(&m_variableTable[index].data));
        m_stack.pop();
    }
    inline void D_STORE(){
        MNEMONIC("D_STORE");
        uint index = *getNextByte();
        m_variableTable[index] = m_stack.top();
        DEBUG("slot: " << index << " value: " << m_variableTable[index].data);       
        m_stack.pop();
    }
    inline void L_STORE(){                                              // THIS REALLY DOESN'T WORK
        MNEMONIC("L_STORE");
        uint index = *getNextByte();
        m_variableTable[index] = m_stack.top();
        DEBUG("slot: " << index << " value: " << m_variableTable[index].data);       
        m_stack.pop();
    }
    inline void REF_STORE(){
        MNEMONIC("REF_STORE");
        uint index = *getNextByte();
        m_variableTable[index] = m_stack.top();
        DEBUG("slot: " << index << " ref: 0x" << std::hex << m_variableTable[index].data << std::dec);
        m_stack.pop();
    }
    inline void SB_LOAD(){
        MNEMONIC("SB_LOAD");
        uint index = *getNextByte();
        m_stack.push({_SBYTE_, m_variableTable[index].data & 0xff});
        DEBUG("slot: " << index << " value: " << (m_stack.top().data & 0xff));
    }
    inline void UB_LOAD(){
        MNEMONIC("UB_LOAD");
        uint index = *getNextByte();
        m_stack.push({_UBYTE_, m_variableTable[index].data & 0xff});
        DEBUG("slot: " << index << " value: " << (m_stack.top().data & 0xff));
    }
    inline void SI_LOAD(){
        MNEMONIC("SI_LOAD");
        uint index = *getNextByte();
        m_stack.push(m_variableTable[index]);
        DEBUG("slot: " << index << " value: " << (m_stack.top().data & 0xff));
    }
    inline void UI_LOAD(){
        MNEMONIC("UI_LOAD");
        uint index = *getNextByte();
        m_stack.push(m_variableTable[index]);
        DEBUG("slot: " << index << " value: " << (m_stack.top().data & 0xff));
    }
    inline void F_LOAD(){
        MNEMONIC("F_LOAD");
        uint index = *getNextByte();
        m_stack.push(m_variableTable[index]);
        DEBUG("slot: " << index << " value: " << (m_stack.top().data & 0xff));
    }
    inline void D_LOAD(){
        MNEMONIC("D_LOAD");
        uint index = *getNextByte();
        m_stack.push(m_variableTable[index]);
        DEBUG("slot: " << index << " value: " << *reinterpret_cast<double*>(&m_stack.top().data));
    }
    inline void L_LOAD(){                   // This does not work. Am I going to fix it. No.
        MNEMONIC("L_LOAD");
        uint index = *getNextByte();
        m_stack.push(m_variableTable[index]);
        DEBUG("slot: " << index << " value: " << m_stack.top().data);
    }
    inline void REF_LOAD(){
        MNEMONIC("REF_LOAD");
        uint index = *getNextByte();
        m_stack.push(m_variableTable[index]);
        DEBUG("slot: " << index << " ref: 0x" << std::hex << m_stack.top().data << std::dec);
    }
    inline void POP(){
        MNEMONIC("POP");
        m_stack.pop();
    }
    inline void POP2(){
        MNEMONIC("POP2");
        m_stack.pop();
        m_stack.pop();
    }
    inline void DUP(){
        MNEMONIC("DUP");
        m_stack.push(m_stack.top());
    }
    inline void DUP2(){
        MNEMONIC("DUP2");
        container temp = m_stack.top();
        m_stack.pop();
        container temp2 = m_stack.top();
        m_stack.push(temp);
        m_stack.push(temp2);
        m_stack.push(temp);
    }
    inline void DUP2X(){
        MNEMONIC("DUP2X");
        m_stack.push(m_stack.top());
        m_stack.push(m_stack.top());
    }
    inline void SWAP(){
        MNEMONIC("SWAP");
        container a = m_stack.top();
        m_stack.pop();
        std::swap(a, m_stack.top());
        m_stack.push(a);
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
        DEBUG(std::hex << (uint)address << std::dec);
        JMP_HELPER(address);
    }
    inline void JIFNE(){
        uint temp = m_stack.top().data;
        byte address = *getNextByte();
        m_stack.pop();
        MNEMONIC("JIFNE");
        DEBUG(temp << " != " << m_stack.top().data << " " << std::hex << (uint)address << std::dec);
        if(temp != m_stack.top().data)
            JMP_HELPER(address);
        m_stack.pop();
    }
    inline void JIFE(){
        uint temp = m_stack.top().data;
        byte address = *getNextByte();
        m_stack.pop();
        MNEMONIC("JIFE");
        DEBUG(temp << " == " << m_stack.top().data << " " << std::hex << (uint)address << std::dec);
        if(temp == m_stack.top().data)
            JMP_HELPER(address);
        m_stack.pop();
    }
    inline void JIFLS(){
        uint temp = m_stack.top().data;
        byte address = *getNextByte();
        m_stack.pop();
        MNEMONIC("JIFLS");
        DEBUG(temp << " < " << m_stack.top().data << " " << std::hex << (uint)address << std::dec);
        if(temp < m_stack.top().data)
            JMP_HELPER(address);
        m_stack.pop();
    }
    inline void JIFGT(){
        uint temp = m_stack.top().data;
        byte address = *getNextByte();
        m_stack.pop();
        MNEMONIC("JIFGT");
        DEBUG(temp << " > " << m_stack.top().data << " " << std::hex << (uint)address << std::dec);
        if(temp > m_stack.top().data)
            JMP_HELPER(address);
        m_stack.pop();
    }
    inline void JIFZ(){
        byte address = *getNextByte();
        MNEMONIC("JIFZ");
        DEBUG(m_stack.top().data << " == 0 " << std::hex << (uint)address << std::dec);
        if(m_stack.top().data == 0)
            JMP_HELPER(address);
        m_stack.pop();
    }
    inline void JIFNZ(){
        byte address = *getNextByte();
        MNEMONIC("JIFNZ");
        DEBUG(m_stack.top().data << " != 0 " << std::hex << (uint)address << std::dec);
        if(m_stack.top().data != 0)
            JMP_HELPER(address);
        m_stack.pop();
    }
};