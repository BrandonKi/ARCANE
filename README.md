The old version of ARCANE has been moved.
It had a functional front-end which supported about half of the major features I plan to do.
However I've decided to rewrite everything mainly because
of bad coding habits and structure early on which made it 
extremely hard to add new features and extend existing ones.

# ARCANE
ARCANE Runs, Compiles, And Never Explodes

A programming language built on a bytecode VM. (ARCVM)

WIP

Tested on Windows 10 and multiple Linux distros with gcc and clang

# Sample Code

```zig
fn main(): int{
    i := 0;
    x : int = 5;
    result := add(i, x);
    ret result;
}
fn add(a : int, b : int): int {
    ret a + b;
}
```
Syntax highlighting is a bit off. Use this extension for correct highlighting in VSCode.
Syntax Highlighting : https://github.com/BrandonKi/arcane-syntax-highlighter

# Trello

https://trello.com/b/tZJWGk7b/arcane

# Notes

All type checking is done at compile time

ARCVM expects correct semantics

Bytecode files store variables in Little Endian 

### RESOURCES FOR LATER

- https://www.scadacore.com/tools/programming-calculators/online-hex-converter/
- https://en.wikipedia.org/wiki/Java_class_file
- https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures
- https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/lectures/13/Slides13.pdf

