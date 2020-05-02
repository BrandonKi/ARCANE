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
	std::stack<byte*> m_stack;
    byte* m_data;
    uint& m_size;
    uint m_memptr;
    VAR* m_variableTable;
    std::stack<unsigned int> m_tempVarStorage;

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
        m_stack.push(nullptr);
    }
};