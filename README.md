# ATN
 A basic programing language.
 Uses a bytecode VM to run.

                        
 Bytecode files store variables in Little Endian 

No type checking at runtime.
Type checking should/will be done at compile time.
Only checks for fatal errors at runtime.


### TODO:

- bounds checks on variable table
- make a function table
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

### DONE:

- file signature
- bytecode file writer
- option to output readable bytecode alongside actual bytecode
- start spreadsheet for all instructions
- push instruction
- variable representation in bytecode
- main execution loop
- debug and release modes
