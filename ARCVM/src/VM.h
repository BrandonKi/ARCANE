#include <iostream>
#include <cstring>
#include <stack>

#include "../debug/MNEMONICS.h"

#define DEBUG_BUILD
// #define RELEASE_BUILD

#define NEWLINE std::cout << "\n"
#define log(x) std::cout << x
#define logn(x) std::cout << x << "\n"
#define ERR(x) std::cout << x << "\n"; std::cin.get(); return -1

#define INT_SIZE 4         // This macro doesn't do anything

#ifdef DEBUG_BUILD
    #define POINTER(x) std::cout << "    POINTER: " << x << "\n"
    #define INSTRUCTION(x) std::cout << "INSTRUCTION: " << x << "\n"
    #define MNEMONIC(x) std::cout << "   MNEMONIC: " << x << "\n"
    #define DEBUG(x) std::cout <<  " DEBUG INFO: " << x << "\n"
    #define WAIT_FOR_INPUT std::cin.get()
    #define DEBUG_STACK printStack()
    #define EXIT_MSG(x) std::cout << "EXITED WITH CODE: " << x << "\n"
#else
    #define POINTER(x)
    #define INSTRUCTION(x)
    #define MNEMONIC(x)
    #define DEBUG(x)
    #define WAIT_FOR_INPUT
    #define DEBUG_STACK
    #define EXIT_MSG(x)
#endif

typedef uint_fast32_t uint;
typedef int_fast32_t sint;
typedef int_fast64_t slong;
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
    uint getNextFourBytes();
    void nextInstruction();
    void executeInstruction();

public:
    VM(char* data, uint size);
    inline ~VM(){delete[] m_data;};
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
        DEBUG(std::dec << (sint)m_stack.top().data);
    }
    inline void UBCONST_PUSH(){
        MNEMONIC("UBCONST_PUSH");
        m_stack.push({_UBYTE_, (uint)*getNextByte()});
        DEBUG(std::dec << (uint)m_stack.top().data);
    }
    inline void SICONST_PUSH(){
        MNEMONIC("SICONST_PUSH");
        m_stack.push({_SINT_, *reinterpret_cast<uint*>(&m_data[++m_memptr])});
        m_memptr += 3;                                                    // m_memptr += INT_SIZE - 1 bcuz I said so
        DEBUG(std::dec << (sint)m_stack.top().data);
    }
    inline void UICONST_PUSH(){
        MNEMONIC("UICONST_PUSH");
        m_stack.push({_UINT_, *reinterpret_cast<uint*>(&m_data[++m_memptr])});
        m_memptr += 3;                                                    // m_memptr += INT_SIZE - 1 bcuz I said so
        DEBUG(std::dec << (uint)m_stack.top().data);
    }
    inline void FCONST_PUSH(){
        MNEMONIC("FCONST_PUSH");
        m_stack.push({_FLOAT_, *reinterpret_cast<uint*>(&m_data[++m_memptr])});
        m_memptr += 3;
        DEBUG(std::dec << *reinterpret_cast<float*>(&m_stack.top().data));
    }
    inline void DCONST_PUSH(){                                                              // FIX THIS
        MNEMONIC("DCONST_PUSH");
    }
    inline void LCONST_PUSH(){                                                              // FIX THIS
        MNEMONIC("LCONST_PUSH");
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
    inline void D_ADD(){                      // FIX THIS
        MNEMONIC("D_ADD");
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
        result = m_stack.top().data - result;
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
        MNEMONIC("D_SUB");                         // FIX THIS
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
        MNEMONIC("D_MUL");                  // FIX THIS
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
        MNEMONIC("D_DIV");              // FIX THIS
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
        float result = *reinterpret_cast<float*>(&m_stack.top().data);
        m_stack.pop();
        result = std::fmodf(*reinterpret_cast<float*>(&m_stack.top().data), result);
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_FLOAT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void D_REM(){
        MNEMONIC("D_REM");              // FIX THIS
    }
    inline void L_REM(){
        MNEMONIC("L_REM");              // FIX THIS
    }
    inline void SB_STORE(){
        MNEMONIC("SB_STORE");
        uint temp = m_stack.top().data;
        m_variableTable[temp] = {_SBYTE_, (*reinterpret_cast<uint*>(&m_stack.top().data) & 0xff)};               // take the 8 least significant bits by using bitmask 0xff and & to convert to byte
        DEBUG("slot " << temp << " value " << (sint)(int_fast8_t)(m_variableTable[temp].data & 0xff));           // I could just do % BYTE_MAX_VALUE but no
        m_stack.pop();
    }
    inline void UB_STORE(){
        MNEMONIC("UB_STORE");
        uint temp = m_stack.top().data;
        m_variableTable[temp] = {_UBYTE_, (m_stack.top().data & 0xff)};                     // take the 8 least significant bits by using bitmask 0xff and & to convert to byte
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);              // I could just do % BYTE_MAX_VALUE but no
        m_stack.pop();
    }
    inline void SI_STORE(){
        MNEMONIC("SI_STORE");
        uint temp = m_stack.top().data;
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << (sint)(m_variableTable[temp].data));       
        m_stack.pop();
    }
    inline void UI_STORE(){
        MNEMONIC("UI_STORE");
        uint temp = m_stack.top().data;
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);       
        m_stack.pop();
    }
    inline void F_STORE(){                                              // THIS DOESN'T WORK
        MNEMONIC("F_STORE");
        uint temp = m_stack.top().data;
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << *reinterpret_cast<float*>(&m_variableTable[temp].data));
        m_stack.pop();
    }
    inline void D_STORE(){                                              // THIS REALLY DOESN'T WORK
        MNEMONIC("D_STORE");
        uint temp = m_stack.top().data;
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);       
        m_stack.pop();
    }
    inline void L_STORE(){                                              // THIS REALLY DOESN'T WORK
        MNEMONIC("L_STORE");
        uint temp = m_stack.top().data;
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);       
        m_stack.pop();
    }
    inline void SB_LOAD(){
        MNEMONIC("SB_LOAD");
        uint temp = m_stack.top().data;
        m_stack.push({_SBYTE_, m_variableTable[temp].data & 0xff});
        DEBUG(m_stack.top().data);
    }
    inline void UB_LOAD(){
        MNEMONIC("UB_LOAD");
        uint temp = m_stack.top().data;
        m_stack.push(container{_UBYTE_, m_variableTable[temp].data & 0xff});
        DEBUG(m_stack.top().data);
    }
    inline void SI_LOAD(){
        MNEMONIC("SI_LOAD");
        uint temp = m_stack.top().data;
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
    }
    inline void UI_LOAD(){
        MNEMONIC("UI_LOAD");
        uint temp = m_stack.top().data;
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
    }
    inline void F_LOAD(){
        MNEMONIC("F_LOAD");
        uint temp = m_stack.top().data;
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
    }
    inline void D_LOAD(){
        MNEMONIC("D_LOAD");
        uint temp = m_stack.top().data;
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
    }
    inline void L_LOAD(){
        MNEMONIC("L_LOAD");
        uint temp = m_stack.top().data;
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
    }
    inline void POP(){
        m_stack.pop();
    }
    inline void POP2(){
        m_stack.pop();
        m_stack.pop();
    }
    inline void DUP(){
        m_stack.push(m_stack.top());
    }
    inline void DUP2(){
        m_stack.push(m_stack.top());
        m_stack.push(m_stack.top());
    }
    inline void SWAP(){
        container a = m_stack.top();
        m_stack.pop();
        std::swap(a, m_stack.top());
        m_stack.push(a);
    }
    inline void GOTO(){
        byte address = *getNextByte();
        MNEMONIC("GOTO" << std::hex << (uint)address);
        if(address == 0)
            m_memptr = 0xffffffff;          // ugly workaround... haha get it workaround... like integer wraparound...
        else
            m_memptr = address-1;
    }
};