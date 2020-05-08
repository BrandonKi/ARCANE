# ARCANE
ARCANE Runs, Compiles, And Never Explodes

A programming language and bytecode VM. (ARCVM)

There is no type checking at runtime.
Type checking should/will be done at compile time.
Only checks for fatal errors at runtime.

# Notes

Bytecode files store variables in Little Endian 

## TODO:

### VM
- bounds checks on variable table
- make a function table
- const table
- specify amount of variables and functions needed in file
- add all missing instructions for add, store
- push instruction for 64 bit types
- support for 64 bit types (double, long)
- sub instruction
- sub instruction for 64 bit types
- mul instruction
- mul instruction for 64 bit types
- div instruction
- div instruction for 64 bit types
- rem instruction
- rem instruction for 64 bit types
- pop instruction
- 64 bit type representation in bytecode
- string support and representation in bytecode

### Compiler

- lexical analysis
- symbol table construction
- syntax analysis
- semantic analysis
- code generation
- optimization


- UPDATE SPREADSHEET WITH NEW INSTRUCTIONS AND DESCRIPTIONS


## DONE:

- file signature
- bytecode file writer
- option to output readable bytecode alongside actual bytecode
- start spreadsheet for all instructions
- push instruction
- variable representation in bytecode
- main execution loop
- debug and release modes
- file extension for source, bytecode, and readable bytecode


## Compiler explanation

### Lexical analysis/Tokeniztion
- Comments and unnecessary spaces are removed.
- Groups the characters/words into lexemes
- 
### Symbol table construction
- A table stores the names and addresses of all variables, constants and arrays.
- Variables are checked to make sure they have been declared and to determine the data types used.

### Syntax analysis
- Tokens are checked to see if they match the syntax of the programming language.
- If syntax errors are found, error messages are produced.

### Semantic analysis
- Variables are checked to make sure they have been correctly declared and contain the correct data type.
- Operations are checked to ensure that they are appropriate for the type of variable being used.

### Bytecode generation
- Bytecode code is generated in this stage.

### Optimization
- Code optimization makes the program run faster.

### RESOURCES FOR LATER

- https://www.scadacore.com/tools/programming-calculators/online-hex-converter/
- https://en.wikipedia.org/wiki/Java_class_file


