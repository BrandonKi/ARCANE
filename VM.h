#include <iostream>
#include <bitset>
#include <algorithm>
#include <stack>

#define NEWLINE std::cout << "\n"

typedef unsigned int uint;
typedef unsigned char byte;
typedef std::bitset<8> rawByte;

enum Type {BOOL, INT, UINT, CHAR, UCHAR, FLOAT, DOUBLE};

struct VAR {
	Type type;
	union{
		bool a;
		int b;
	};
};

class VM {

private:
	std::stack<byte*> m_stack;
    byte* m_data;
    uint& m_size;
    uint m_memptr;
    VAR* m_variableTable;

    byte* convertToByteArray(char*);
    void printStack();
    inline byte* getNextByte();
    inline void nextInstruction();
    inline void executeInstruction();

public:
    VM(char* data, uint size);
    inline ~VM(){delete[] m_data;};
    void printProgram(byte*);
    inline byte* getProgram(){return m_data;};
    void run();
};