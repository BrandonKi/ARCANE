# ARCANE
ARCANE is an acronym for
ARCANE Runs, Compiles, And Never Explodes

A programming language built on a bytecode VM. (ARCVM)
https://github.com/BrandonKi/ARCVM-V2

C-like language but without direct memory access.

WIP

# Sample Code

```zig
fn add(a : int, b : int) int {
    ret a + b;
}

fn main() int {
    i := 0;
    x : int = 5;
    result := add(i, x);
    ret result;
}
```
Syntax Highlighting in VSCode: https://github.com/BrandonKi/arcane-syntax-highlighter

## Build/Run

```
git clone https://github.com/BrandonKi/ARCANE
cd ARCANE
mkdir build
cd build
cmake ..
ninja all
cd ../bin
./ARCC <filename>
```
