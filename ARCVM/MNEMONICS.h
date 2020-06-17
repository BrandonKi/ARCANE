#define _NOP_ 0x00

#define _EXIT_ 0x01

#define _NCONST_PUSH_ 0x02
#define _SBCONST_PUSH_ 0x10
#define _UBCONST_PUSH_ 0x11
#define _SICONST_PUSH_ 0x12
#define _UICONST_PUSH_ 0x13
#define _FCONST_PUSH_ 0x14
#define _DCONST_PUSH_ 0x15
#define _LCONST_PUSH_ 0x16
#define _SCONST_PUSH_ 0x17

#define _ARR_LEN_ 0x40
#define _NEW_ARR_ 0x41
#define _UBA_STORE_ 0x43
#define _UBA_LOAD_ 0x44

#define _SB_ADD_ 0xa0
#define _UB_ADD_ 0xa1
#define _SI_ADD_ 0xa2
#define _UI_ADD_ 0xa3
#define _F_ADD_ 0xa4
#define _D_ADD_ 0xa5
#define _L_ADD_ 0xa6

#define _SB_SUB_ 0xa7
#define _UB_SUB_ 0xa8
#define _SI_SUB_ 0xa9
#define _UI_SUB_ 0xaa
#define _F_SUB_ 0xab
#define _D_SUB_ 0xac
#define _L_SUB_ 0xad

#define _SB_MUL_ 0xb0
#define _UB_MUL_ 0xb1
#define _SI_MUL_ 0xb2
#define _UI_MUL_ 0xb3
#define _F_MUL_ 0xb4
#define _D_MUL_ 0xb5
#define _L_MUL_ 0xb6

#define _SB_DIV_ 0xb7
#define _UB_DIV_ 0xb8
#define _SI_DIV_ 0xb9
#define _UI_DIV_ 0xba
#define _F_DIV_ 0xbb
#define _D_DIV_ 0xbc
#define _L_DIV_ 0xbd

#define _SB_REM_ 0xc0
#define _UB_REM_ 0xc1
#define _SI_REM_ 0xc2
#define _UI_REM_ 0xc3
#define _F_REM_ 0xc4
#define _D_REM_ 0xc5
#define _L_REM_ 0xc6

#define _SB_STORE_ 0xc7
#define _UB_STORE_ 0xc8
#define _SI_STORE_ 0xc9
#define _UI_STORE_ 0xca
#define _F_STORE_ 0xcb
#define _D_STORE_ 0xcc
#define _L_STORE_ 0xcd
#define _REF_STORE_ 0xce

#define _SB_LOAD_ 0xd0
#define _UB_LOAD_ 0xd1
#define _SI_LOAD_ 0xd2
#define _UI_LOAD_ 0xd3
#define _F_LOAD_ 0xd4
#define _D_LOAD_ 0xd5
#define _L_LOAD_ 0xd6
#define _REF_LOAD_ 0xd7

#define _POP_ 0xe0
#define _POP2_ 0xe1

#define _DUP_ 0xe3
#define _DUP2_ 0xe4
#define _DUP2X_ 0xe5

#define _SWAP_ 0xea

#define _JMP_ 0xf0
#define _JIFNE_ 0xf1
#define _JIFE_ 0xf2
#define _JIFLS_ 0xf4
#define _JIFGT_ 0xf5
#define _JIFZ_ 0xf6
#define _JIFNZ_ 0xf7





#define _NULL_ 0
#define _SBYTE_ 1
#define _UBYTE_ 2
#define _SINT_ 3
#define _UINT_ 4
#define _FLOAT_ 5
#define _DOUBLE_ 6
#define _LONG_ 7
#define _STRING_ 8
#define _REF_ 9
#define _SPECIAL_ 10
#define _UNDEFINED_ 11