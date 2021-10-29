// file for random stuff
// not meant to be included
#if 0

// add error for multiple declaration

// unary expression generation

// distinguish between rvalues and lvalues

// fix variables in code gen

// function calling

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

/*
[entrypoint]
define function main() -> i32 {
  #main
    %0 = alloc i32
    store %0, 0
    %1 = load %0
    brnz %1, #0, #1
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
    store %4, 0
    %5 = load %4
    brnz %5, #3, #4
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
    store %8, 0
    %9 = load %8
    brnz %9, #6, #7
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
}*/

// that can be trivially cleaned up/optimized to be the IR below
// the question is whether this should be done by the front or back end
// I'm leaning torwards a canonicalization pass or a clean up pass that can be used by the front end also for debugging
//
//
// list of stuff to include or put in different passes
// * remove dead control flow instructions
// * inline constants
// * rename auto-generated label/variable names
//
// other stuff that can be done
// * remove dead basic blocks
// * evaluate constant branches
//
/*
[entrypoint]
define function main() -> i32 {
  #main
    brnz 0, #0, #1
  #0
    ret 2
  #1
    brnz 0, #2, #3
  #2
    ret 3
  #3
    brnz 0, #5, #6
  #5
    ret 4
  #6
    ret 5
}
*/
//
// with the extra optimizations it could be this
/*
[entrypoint]
define function main() -> i32 {
  #main
    ret 5
}
*/





#endif