#include "Parser.h"

Parser::Parser(std::vector<RawFile>& projectFiles){  //TODO good candidate for multithreading

    // std::thread worker([](){});
    for(RawFile rf: projectFiles){
        std::cout << rf.filepath << '\n';
        Lexer lexer(rf.filedata);
        data.push_back(LexedFile{rf.filepath, lexer.lex()});
    } 
}

Project* Parser::parse(){
    return parseProject();
}

Project* Parser::parseProject(){    //TODO another good candidate for multithreading
    std::vector<File*> files;
    for(LexedFile& lf : data){
        // update/reset member variables for each file
        index = 0;
        tokens = lf.filedata;
        files.push_back(parseFile());
    }
    return ast.newProjectNode(SourcePos{}, files);
}

File* Parser::parseFile(){

    for(; index < tokens.size(); index++){  
        switch(currentToken()->kind){
            case ARC_IMPORT:
                std::cout << "import statement\n";
                break;
            case ARC_FN:
                std::cout << "function start\n";
                break;
            case ARC_ID:
                parseDecl();
        }
    }
    return new File{};
}

Decl* Parser::parseDecl(){
    SourcePos startPos = currentToken()->pos;
    std::string id = currentToken()->data;
    if(peekNextToken()->kind == ARC_INFER){

    }
    else if(nextToken()->kind == ARC_COLON){
        Type type = tokenKind2Type(currentToken()->kind);
        switch(currentToken()->kind){
            case ARC_I8:
            case ARC_I16:
            case ARC_I32:
            case ARC_I64:
            case ARC_U8:
            case ARC_U16:
            case ARC_U32:
            case ARC_U64:
            case ARC_F32:
            case ARC_F64:
            case ARC_STRUCT:
            case ARC_STR:
            case ARC_ARR:
                if(peekNextToken()->kind == ARC_SEMICOLON){
                    //TODO definition but no declaration
                }
                else if(peekNextToken()->kind == ARC_EQUAL){
                    nextToken();
                    return ast.newDeclNode(startPos, id, type, parseExpr());
                }
                else{
                    //TODO syntax error here
                }
                break;
            default:
                //TODO syntax error here
                errorLog.push(ErrorMessage{});
                break;
        }
    }
    else{
        //TODO syntax error here
        errorLog.push(ErrorMessage{});
    }
    return new Decl{};
}

/**
 * expects the current token to be the first token in the expression
 */
Expr* Parser::parseExpr(){
    std::vector<Token*> result;
    std::vector<Token*> stack;

    while(currentToken()->kind != ARC_SEMICOLON){
        if(peekNextToken()->kind == ARC_INT_LIT){
            
        }
    }
}

inline Token* Parser::currentToken(){
    return tokens[index];
}

inline Token* Parser::nextToken(){
    return tokens[++index];
}

inline Token* Parser::peekNextToken(){
    return tokens[index + 1];
}

Type Parser::tokenKind2Type(TokenKind tkn){
    switch(tkn){
        case ARC_I8:
            return TYPE_I8;
        case ARC_I16:
            return TYPE_I16;
        case ARC_I32:
            return TYPE_I32;
        case ARC_I64:
            return TYPE_I64;
        case ARC_U8:
            return TYPE_U8;
        case ARC_U16:
            return TYPE_U16;
        case ARC_U32:
            return TYPE_U32;
        case ARC_U64:
            return TYPE_U64;
        case ARC_F32:
            return TYPE_F32;
        case ARC_F64:
            return TYPE_F64;
        case ARC_STRUCT:
            return TYPE_STRUCT;
        case ARC_STR:
            return TYPE_STRING;
        case ARC_ARR:
            return TYPE_ARR;
    }
    return (Type)-1;
}