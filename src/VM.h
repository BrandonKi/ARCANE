#include <iostream>
#include <cstring>
#include <stack>

#define DEBUG_BUILD

#define NEWLINE std::cout << "\n"
#define log(x) std::cout << x
#define logn(x) std::cout << x << "\n"
#define ERR(x) std::cout << x << "\n"; return -1

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

class VM {

private:
    byte* m_data;
    uint m_size;
    uint m_memptr;
    uint* m_variableTable;
    std::stack<unsigned int> m_stack;

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
    }
    inline void SBCONST_PUSH(){
        MNEMONIC("SBCONST_PUSH");
        m_stack.push(*getNextByte());
        DEBUG(std::dec << (int)m_stack.top());
    }
    inline void UBCONST_PUSH(){
        MNEMONIC("UBCONST_PUSH");
        m_stack.push((uint)*getNextByte());
        DEBUG(std::dec << (uint)m_stack.top());
    }
    inline void SICONST_PUSH(){
        MNEMONIC("SICONST_PUSH");
        m_stack.push(*reinterpret_cast<uint*>(&m_data[++m_memptr]));
        m_memptr += 3;                                                    // INT_SIZE - 1 bcuz I said so
        DEBUG(std::dec << (int)m_stack.top());
    }
    inline void UICONST_PUSH(){
        MNEMONIC("UICONST_PUSH");
        m_stack.push(*reinterpret_cast<uint*>(&m_data[++m_memptr]));
        m_memptr += 3;                                                    // INT_SIZE - 1 bcuz I said so
        DEBUG(std::dec << (uint)m_stack.top());
    }
    inline void FCONST_PUSH(){
        MNEMONIC("FCONST_PUSH");
        m_stack.push(*reinterpret_cast<uint*>(&m_data[m_memptr]));
        m_memptr += 3;
        DEBUG(std::dec << (float)m_stack.top());
    }
    inline void DCONST_PUSH(){                                           // FIX THIS
        MNEMONIC("DCONST_PUSH");
        m_stack.push(*reinterpret_cast<uint*>(&m_data[m_memptr]));
        m_memptr += 8;
        DEBUG(std::dec << (double)m_stack.top());
    }
    inline void LCONST_PUSH(){                                           // FIX THIS
        MNEMONIC("LCONST_PUSH");
        m_stack.push(*reinterpret_cast<uint*>(&m_data[m_memptr]));
        m_memptr += 8;
        DEBUG(std::dec << (double)m_stack.top());
    }
    inline void SB_ADD(){
        MNEMONIC("SB_ADD");
        uint result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push(result);
    }
    inline void UB_ADD(){
        MNEMONIC("UB_ADD");
        uint result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push(result);
    }
    inline void SI_ADD(){
        MNEMONIC("SI_ADD");
        int result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push(result);
    }
    inline void UI_ADD(){
        MNEMONIC("UI_ADD");
        uint result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push(result);
    }
    inline void F_ADD(){
        MNEMONIC("F_ADD");
        float result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push(result);
    }
    inline void D_ADD(){                      // FIX THIS
        MNEMONIC("D_ADD");
        double result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push(result);
    }
    inline void L_ADD(){                      // FIX THIS
        MNEMONIC("L_ADD");
        long long int result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push(result);
    }
    inline void SB_STORE(){
        MNEMONIC("SB_STORE");
        int temp = *getNextByte();
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_stack.top());
        m_stack.pop();
    }
    inline void UB_STORE(){
        MNEMONIC("UB_STORE");
        int temp = *getNextByte();
        m_variableTable[temp] = m_stack.top();
        DEBUG("slot " << temp << " value " << m_stack.top());
        m_stack.pop();
    }
    inline void GOTO(){
        byte address = *getNextByte();
        MNEMONIC("GOTO" << std::hex << (uint)address);
        if(address == 0)
            m_memptr = 0xffffffff;       // ugly workaround... haha get it workaround... like integer wraparound...
        else
            m_memptr = address-1;
    }
};