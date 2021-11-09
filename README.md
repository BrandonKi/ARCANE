# ARCANE
ARCANE is an acronym for
ARCANE Runs, Compiles, And Never Explodes

A programming language built on a custom backend. (ARCVM)
https://github.com/BrandonKi/ARCVM

(Work In Progress)

# Sample Code

```zig
fn add(a : int, b : int) int {
    ret a + b;
}

fn main() int {
    i := 0;
    x : int = 5;
    result := add(i, x);
    // supports UFCS
    result := i.add(x);
    ret result;
}
```
Syntax Highlighting in VSCode: https://github.com/BrandonKi/arcane-syntax-highlighter
