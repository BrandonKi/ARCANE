#include <fstream>

#include "MNEMONICS.h"

int main(){


     unsigned char data[] = 
    {
        0x42,           // start file signature
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
        0x68,           // end file signature

                        // start of function table
        0x01,           // number of functions
                        
        'm',
        'a',
        'i',
        'n',
        0x00,
        
        _CALL_LOCAL_,   // bootstrap code
        0x00,
        0x00,
        0x00,
        0x00,

        _EXIT_,         // exit

                        // start main
        'm',
        'a',
        'i',
        'n',
        0x00,
        
        0x01,           // local variable count
        0x00,           // arg count

        //0x02,           // arg types
                    
        _SICONST_PUSH_,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        _SI_STORE_,
        0x01,
        _SI_LOAD_,
        0x01,

        _RET_,
                        // end main
    };

    std::ofstream file("example.arcb", std::ios::out|std::ios::binary|std::ios::trunc);
    if(file.is_open())
        file.write((char *)data, sizeof(data));  
    file.close();
}

    // unsigned char data[] = 
    // {
    //     0x42,           // start file signature
    //     0x72,
    //     0x61,
    //     0x6e,
    //     0x64,
    //     0x6f,
    //     0x6e,
    //     0x4b,
    //     0x69,
    //     0x72,
    //     0x69,
    //     0x6e,
    //     0x63,
    //     0x69,
    //     0x63,
    //     0x68,           // end file signature

    //                     // start of function table
    //     0x02,           // number of functions
                        
    //     'm',
    //     'a',
    //     'i',
    //     'n',
    //     0x00,

    //     'a',
    //     'd',
    //     'd',
    //     0x00,
        
    //     _CALL_LOCAL_,   // bootstrap code
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,

    //     _EXIT_,         // exit

    //                     // start main
    //     'm',
    //     'a',
    //     'i',
    //     'n',
    //     0x00,
        
    //     0x00,           // local variable count
    //     0x00,           // arg count

    //     //0x02,           // arg types
                    
    //     _UBCONST_PUSH_,
    //     0x04,
    //     _UBCONST_PUSH_,
    //     0x02,
    //     _CALL_LOCAL_,
    //     0x01,
    //     0x00,
    //     0x00,
    //     0x00,

    //     _RET_,
    //                     // end main
        

    //                     // start add
    //     'a',
    //     'd',
    //     'd',
    //     0x00,

    //     0x02,           // local variable count
    //     0x02,           // arg count

    //     0x02,           // arg types
    //     0x02,

    //     _UB_LOAD_,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     _UB_LOAD_,
    //     0x01,
    //     0x00,
    //     0x00,
    //     0x00,
    //     _UB_ADD_,
    //     _RET_,

    // };