// file for random stuff
// not meant to be included
#if 0

// register context abstraction

// add error for multiple declaration

// unary expression generation

// distinguish between rvalues and lvalues

// fix variables in code gen

// pass arguements to function

// var args

// default args

// named args

// ? multiple return values ?

// ? multiple assignment ?

// clean up generated IR for conditionals; there is a lot of redundant IR

// constants/literals result in redundant IR

// maybe run a small cleanup pass before printing IR for debug
// just removing the useless br instruction and merging bblocks with only a br
// otherwise IR like below is generated

//~ DONE
// basic function calling
// support elif chains
// true/false literals


[entrypoint]
define function main() -> i32 {
  #main
  #loop init
    %0 = alloc i32
    %1 = alloc i32
    store %1, 1
    %2 = load %1
    store %0, %2
    %3 = load %0
  #loop_cmp
    %4 = load %0
    %5 = alloc i32
    store %5, 10
    %6 = load %5
    %7 = lt %4, %6
    brnz %7, #loop_cmp, #post loop
  #loop body
    %8 = alloc i32
    %9 = alloc i32
    store %9, 1
    %10 = load %9
    store %8, %10
    %11 = load %8
  #loop inc
    %12 = add %3, 1
    br #loop_cmp
  #post loop
    %13 = alloc i32
    store %13, 9
    %14 = load %13
    ret %14
}


#endif