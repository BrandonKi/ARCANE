#include "Parser.h" 



struct Label{
    std::string label;
    unsigned int pos;
    bool is_fn;
};

class CodeGen{

    private:
        std::vector<Label> label_table;
        std::vector<Quad> m_table;
        std::vector<value> m_lcl_var_table;
        unsigned int current_pos;
        unsigned int local_var_index;//@TODO Variables are not supported outside functions yet. Need to implement in the future.
        std::vector<unsigned char> const_table;
        std::vector<unsigned char> code;
        
        unsigned char file_signature[16] = {
            0x42,  
            0x72,
            0x61,
            0x6e,
            0x64,
            0x6f,
            0x6e,
            0x4b,
            0x69,
            0x72,
            0x69,
            0x6e,
            0x63,
            0x69,
            0x63,
            0x68
        };

        unsigned char bootstrap[6] = 
        {
            _CALL_LOCAL_,
            0x00,
            0x00,
            0x00,
            0x00,
            _EXIT_
        };

    public:
        CodeGen(std::vector<Quad> table): m_table(table), current_pos(0), local_var_index(0) { label_table.reserve(30); code.reserve(100); m_lcl_var_table.reserve(20), const_table.reserve(50);}
        void generate();


    private:
        void moveMain();

        void genFnCall();
        void genFunction();
        void genTmpVar();
        void genVar();
        void genRet();
        void genLabel();

        void loadVar(std::string&);

        void genConstTable();

        void push_string(std::string&);
        void push_int64(int64_t);
        int64_t str_to_int64(std::string&);

        void writeFile();
};