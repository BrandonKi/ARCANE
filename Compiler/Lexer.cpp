#include "Lexer.h"

Lexer::Lexer(const char* filedata, unsigned int length){
    tokens.reserve(300);
    for(unsigned int i = 0; i < length; i++){
        log(*(filedata+i));
        switch(*(filedata+i)){
            CASE_VALID_NAME_CHARACTERS
                i = handleID();
        }
    }
}

unsigned int handleID(){
    
}