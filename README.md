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



Infix to Postfix Rules

A summary of the rules with parentheses follows:

1. Print operands as they arrive.
2. If the stack is empty or contains a left parenthesis on top, push the incoming operator onto the stack.
3. If the incoming symbol is a left parenthesis, push it on the stack.
4. If the incoming symbol is a right parenthesis, pop the stack and print the operators until you see a left
parenthesis. Discard the pair of parentheses.
5. If the incoming symbol has higher precedence than the top of the stack, push it on the stack.
6. If the incoming symbol has equal precedence with the top of the stack, use association. If the association is
left to right, pop and print the top of the stack and then push the incoming operator. If the association is right to
left, push the incoming operator.
7. If the incoming symbol has lower precedence than the symbol on the top of the stack, pop the stack and
print the top operator. Then test the incoming operator against the new top of the stack.
8. At the end of the expression, pop and print all operators on the stack. (No parentheses should remain.)