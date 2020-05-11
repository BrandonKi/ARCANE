#include <iostream>
#include <cstring>
#include <stack>

#include "../debug/MNEMONICS.h"

#define DEBUG_BUILD

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
    #define EXIT_MSG(x) std::cout << "EXITED WITH CODE: " << x << "\n"
#else
    #define POINTER(x)
    #define INSTRUCTION(x)
    #define MNEMONIC(x)
    #define DEBUG(x)
    #define EXIT_MSG(x)
#endif

typedef unsigned int uint;
typedef unsigned char byte;

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
        DEBUG(std::dec << (int)m_stack.top().data);
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
        DEBUG(std::dec << (int)m_stack.top().data);
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
        DEBUG(std::dec << (float)m_stack.top().data);
    }
    inline void DCONST_PUSH(){                                                              // FIX THIS
        MNEMONIC("DCONST_PUSH");
        m_stack.push({_DOUBLE_, *reinterpret_cast<uint*>(&m_data[m_memptr])});
        m_memptr += 8;
        DEBUG(std::dec << (double)m_stack.top().data);
    }
    inline void LCONST_PUSH(){                                                              // FIX THIS
        MNEMONIC("LCONST_PUSH");
        m_stack.push({_LONG_, *reinterpret_cast<uint*>(&m_data[m_memptr])});
        m_memptr += 8;
        DEBUG(std::dec << (double)m_stack.top().data);
    }
    inline void SB_ADD(){
        MNEMONIC("SB_ADD");
        uint result = (m_stack.top().data & 0xff);
        m_stack.pop();
        result += (m_stack.top().data & 0xff);
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_SBYTE_, result});
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
        int result = m_stack.top().data;
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
        float result = m_stack.top().data;
        m_stack.pop();
        result += m_stack.top().data;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_FLOAT_, *reinterpret_cast<uint*>(&result)});
    }
    inline void D_ADD(){                      // FIX THIS
        MNEMONIC("D_ADD");
        double result = m_stack.top().data;
        m_stack.pop();
        result += m_stack.top().data;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_DOUBLE_, *reinterpret_cast<uint*>(&result)});
    }
    inline void L_ADD(){                      // FIX THIS
        MNEMONIC("L_ADD");
        long long int result = m_stack.top().data;
        m_stack.pop();
        result += m_stack.top().data;
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push({_LONG_, *reinterpret_cast<uint*>(&result)});
    }
    inline void SB_STORE(){
        MNEMONIC("SB_STORE");
        int temp = *getNextByte();
        m_variableTable[temp] = {_SBYTE_, (m_stack.top().data & 0xff)};                     // take the 8 least significant bits by using bitmask 0xff and & to convert to byte
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);            // I could just do % BYTE_MAX_VALUE but no
        m_stack.pop();
    }
    inline void UB_STORE(){
        MNEMONIC("UB_STORE");
        int temp = *getNextByte();
        m_variableTable[temp] = {_UBYTE_, (m_stack.top().data & 0xff)};                     // take the 8 least significant bits by using bitmask 0xff and & to convert to byte
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);       // I could just do % BYTE_MAX_VALUE but no
        m_stack.pop();
    }
    inline void SI_STORE(){
        MNEMONIC("SI_STORE");
        int temp = *getNextByte();
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);       
        m_stack.pop();
    }
    inline void UI_STORE(){
        MNEMONIC("UI_STORE");
        int temp = *getNextByte();
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);       
        m_stack.pop();
    }
    inline void F_STORE(){                                              // THIS DOESN'T WORK
        MNEMONIC("F_STORE");
        int temp = *getNextByte();
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);       
        m_stack.pop();
    }
    inline void D_STORE(){                                              // THIS REALLY DOESN'T WORK
        MNEMONIC("D_STORE");
        int temp = *getNextByte();
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);       
        m_stack.pop();
    }
    inline void L_STORE(){                                              // THIS REALLY DOESN'T WORK
        MNEMONIC("L_STORE");
        int temp = *getNextByte();
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_variableTable[temp].data);       
        m_stack.pop();
    }
    inline void SB_LOAD(){
        MNEMONIC("SB_LOAD");
        int temp = *getNextByte();
        m_stack.push({_SBYTE_, m_variableTable[temp].data & 0xff});
        DEBUG(m_stack.top().data);
    }
    inline void UB_LOAD(){
        MNEMONIC("UB_LOAD");
        int temp = *getNextByte();
        m_stack.push(container{_UBYTE_, m_variableTable[temp].data & 0xff});
        DEBUG(m_stack.top().data);
    }
    inline void SI_LOAD(){
        MNEMONIC("SI_LOAD");
        int temp = *getNextByte();
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
    }
    inline void UI_LOAD(){
        MNEMONIC("UI_LOAD");
        int temp = *getNextByte();
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
    }
    inline void F_LOAD(){
        MNEMONIC("F_LOAD");
        int temp = *getNextByte();
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
    }
    inline void D_LOAD(){
        MNEMONIC("D_LOAD");
        int temp = *getNextByte();
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
    }
    inline void L_LOAD(){
        MNEMONIC("L_LOAD");
        int temp = *getNextByte();
        m_stack.push(m_variableTable[temp]);
        DEBUG(m_stack.top().data);
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