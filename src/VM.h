#include <iostream>
#include <bitset>
#include <algorithm>
#include <stack>

#define NEWLINE std::cout << "\n"
#define log(x) std::cout << x
#define logn(x) std::cout << x << "\n"

typedef unsigned int uint;
typedef unsigned char byte;
typedef std::bitset<8> rawByte;

enum Type {BOOL, INT, UINT, CHAR, UCHAR, FLOAT, DOUBLE};

struct VAR {
	Type type;
	union{
		bool b;
		int i;
        unsigned int ui;
        char c;
        unsigned char uc;
        float f;
        double d;
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

    byte* convertToByteArray(char*);
    void printStack();
    void printTempStack();
    void printVariableTable();
    byte getNextByte();
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
};