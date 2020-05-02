#include <iostream>
#include <cstring>
#include <bitset>
#include <algorithm>
#include <stack>


#define NEWLINE std::cout << "\n"
#define log(x) std::cout << x
#define logn(x) std::cout << x << "\n"
#define ERR(x) std::cout << x << "\n"; return -1

#define POINTER(x) std::cout << "    POINTER: " << x << "\n"
#define INSTRUCTION(x) std::cout << "INSTRUCTION: " << x << "\n"
#define MNEMONIC(x) std::cout << "   MNEMONIC: " << x << "\n"
#define DEBUG(x) std::cout <<  " DEBUG INFO: " << x << "\n"
#define EXIT_MSG(x) std::cout << "EXITED WITH CODE: " << x << "\n"

typedef unsigned int uint;
typedef unsigned char byte;
typedef std::bitset<8> rawByte;

enum Type {UNDEFINED, BOOL, INT, UINT, CHAR, UCHAR, FLOAT, DOUBLE};

struct VAR {
	uint type;
	union{
		bool b;
		int i;
        unsigned int ui;
        char c;
        unsigned char uc;
        float f;
        double d;
        int u;
	};
};

class VM {

private:
    byte* m_data;
    uint& m_size;
    uint m_memptr;
    VAR* m_variableTable;
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
    void store(Type type, uint val, byte index);

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
        int* tempptr = reinterpret_cast<int*>(&m_data[m_memptr]);
        DEBUG(*tempptr);
        m_stack.push(getNextFourBytes());
        int temp = *reinterpret_cast<int*>(&m_stack.top());
        DEBUG(std::dec << temp);
    }
    inline void UICONST_PUSH(){
        MNEMONIC("UICONST_PUSH");
        m_stack.push(getNextFourBytes());
        uint temp = *reinterpret_cast<uint*>(&m_stack.top());
        DEBUG(std::dec << temp);
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
    inline void D_ADD(){
        MNEMONIC("D_ADD");
        double result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        DEBUG(std::dec << result);
        m_stack.push(result);
    }
    inline void STORE_SUM_TWO_BYTES_SIGNED(){
        MNEMONIC("STORE_SUM_TWO_BYTES_SIGNED");
        uint result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        byte index = *getNextByte();
        DEBUG(std::dec << "Index " << (int)index);
        DEBUG(std::dec << m_variableTable[(int)index].type);
        store(Type(CHAR), result, index);
        DEBUG(std::hex << (int)m_variableTable[index].c);
    }
    inline void STORE_SUM_TWO_BYTES_UNSIGNED(){
        MNEMONIC("STORE_SUM_TWO_BYTES_UNSIGNED");
        uint result = m_stack.top();
        m_stack.pop();
        result += m_stack.top();
        m_stack.pop();
        DEBUG(std::dec << result);
        store(Type(UCHAR), result, *getNextByte());
        log(m_variableTable[0].uc);
    }
    inline void GOTO(){
        m_data[m_memptr] = 0x00;
        m_data[m_memptr+1] = 0x00;
        byte address = *getNextByte();
        MNEMONIC("GOTO" << address);
        m_memptr = address;
    }
};