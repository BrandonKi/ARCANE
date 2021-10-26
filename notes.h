// file for random stuff
// not meant to be included
#if 0

// var args

// default args

// named args

// ? multiple return values ?

// ? multiple assignment ?

// DONE
// true/false literals

// DONE
// support elif chains

// clean up generated IR for conditionals; there is a lot of redundant IR

// constants/literals result in redundant IR

// maybe run a small cleanup pass before printing IR for debug
// just removing the useless br instruction and merging bblocks with only a br
// otherwise IR like below is generated
[entrypoint]
define function main() -> i32 {
#main
    %0 = alloc i32
    store %0, 1
    %1 = load %0
    brz %1, #0, #1
#0
    %2 = alloc i32
    store %2, 2
    %3 = load %2
    ret %3
    br #2
#1
    br #2
#2
    %4 = alloc i32
    store %4, 1
    %5 = load %4
    brz %5, #3, #4
#3
    %6 = alloc i32
    store %6, 3
    %7 = load %6
    ret %7
    br #5
#4
    br #5
#5
    %8 = alloc i32
    store %8, 1
    %9 = load %8
    brz %9, #6, #7
#6
    %10 = alloc i32
    store %10, 4
    %11 = load %10
    ret %11
    br #8
#7
    br #8
#8
    %12 = alloc i32
    store %12, 5
    %13 = load %12
    ret %13
}


#endif