# ARCANE
ARCANE Runs, Compiles, And Never Explodes

A programming language and bytecode VM. (ARCVM)





# Notes

https://trello.com/b/tZJWGk7b/arcane

Type checking only occurs when using strings.
Otherwise all type checking should/will be done at compile time.
Only checks for fatal errors at runtime.


Bytecode files store variables in Little Endian 

### RESOURCES FOR LATER

- https://www.scadacore.com/tools/programming-calculators/online-hex-converter/
- https://en.wikipedia.org/wiki/Java_class_file


Windows     RCX/XMM0, RDX/XMM1, R8/XMM2, R9/XMM3	
Stack aligned on 16 bytes. 32 bytes shadow space on stack. The specified 8 registers can only be used for parameters 1 through 4. For C++ classes, the hidden "this" parameter is the first parameter, and is passed in RCX.



Linux	RDI, RSI, RDX, RCX, R8, R9, 
Stack aligned on 16 bytes boundary. 128 bytes red zone below stack. The kernel interface uses RDI, RSI, RDX, R10, R8 and R9. In C++, "this" is the first parameter.

Windows:
    RCX, RDX, R8, R9
    XMM0, XMM1, XMM2, XMM3

Linux:
    RDI, RSI, RDX, RCX, R8, R9
