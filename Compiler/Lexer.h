#include <iostream>
#include <fstream>
#include <cstring>


class Lexer{
    public:
        inline Lexer(const char* buf, unsigned int length):m_buf(buf),m_length(length){};
        inline ~Lexer();                                                                // IMPORTANT! delete m_buf if not needed later
        void start();


    private:
        unsigned int m_length;
        const char* m_buf;

};