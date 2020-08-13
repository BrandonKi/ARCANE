# ARCANE
ARCANE Runs, Compiles, And Never Explodes

A programming language and bytecode VM. (ARCVM)

WIP

Tested on Windows 10 and multiple Linux distros

#Sample Code

    i := 0;
    x : int = 5;
    
    result := add(i, x);

    fn add(a : int, b : int): int {
        ret a + b;
    }




# Notes

replace reintrpret cast with safe version
move everything to arena allocator

https://trello.com/b/tZJWGk7b/arcane

Minimal type checking is done at runtime.
Otherwise all type checking should/will be done at compile time.
Only checks for fatal errors at runtime.


Bytecode files store variables in Little Endian 

Syntax Highlighting : https://github.com/BrandonKi/arcane-syntax-highlighter


### RESOURCES FOR LATER

- https://www.scadacore.com/tools/programming-calculators/online-hex-converter/
- https://en.wikipedia.org/wiki/Java_class_file
- https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures
- https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures/13/Slides13.pdf

