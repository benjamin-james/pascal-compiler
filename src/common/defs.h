/* -*- C -*-
 *
 * defs.h
 *
 * Author: Benjamin T James
 */

#ifndef DEFS_H
#define DEFS_H

#define NOPRINT 1024

#define TOKEN_EOF         0
#define TOKEN_ID          1
#define TOKEN_ADDOP       2
#define TOKEN_MULOP       3
#define TOKEN_RELOP       4
#define TOKEN_NUM_REAL    5
#define TOKEN_NUM_INTEGER 6
#define TOKEN_SIGN        7

#define TOKEN_ARRAY      10
#define TOKEN_BEGIN      11
#define TOKEN_DO         12
#define TOKEN_ELSE       13
#define TOKEN_END        14
#define TOKEN_FUNCTION   15
#define TOKEN_IF         16
#define TOKEN_INTEGER    17
#define TOKEN_NOT        18
#define TOKEN_OF         19
#define TOKEN_PROGRAM    20
#define TOKEN_REAL       21
#define TOKEN_THEN       22
#define TOKEN_VAR        23
#define TOKEN_WHILE      24


#define TOKEN_PERIOD     25
#define TOKEN_ELLIPSIS   26
#define TOKEN_LPAREN     27
#define TOKEN_RPAREN     28
#define TOKEN_LBRACKET   29
#define TOKEN_RBRACKET   30
#define TOKEN_ASSIGN     31
#define TOKEN_COLON      32
#define TOKEN_SEMICOLON  33
#define TOKEN_COMMA      34



#define TOKEN_LT         41
#define TOKEN_LEQ        42
#define TOKEN_EQ         43
#define TOKEN_NEQ        44
#define TOKEN_GT         45
#define TOKEN_GEQ        46

#define TOKEN_TIMES      50
#define TOKEN_RDIV       51
#define TOKEN_IDIV       52
#define TOKEN_MOD        53
#define TOKEN_AND        54

#define TOKEN_OR         55
#define TOKEN_PLUS       56
#define TOKEN_MINUS      57

#define LEXERR           99

#define TOKEN_WHITESPACE      1024
#define TOKEN_NEWLINE         1025

#define LEXERR_ID_TOO_LONG        1
#define LEXERR_UNREC_SYM          2
#define LEXERR_INT_TOO_LONG       3
#define LEXERR_MANTIS_TOO_LONG    4
#define LEXERR_FRAC_TOO_LONG      5
#define LEXERR_LEADING_ZERO       6
#define LEXERR_TRAILING_ZERO      7
#define LEXERR_LINE_TOO_LONG      8
#define LEXERR_EXP_TOO_LONG       9
#define LEXERR_NO_EXP             10
#define LEXERR_NO_FRAC            11



#define ID_STRLEN 10

#ifndef LINELEN
#define LINELEN 72
#endif

/* forward declarations */
typedef struct machine *machine_t;
typedef struct lex_state *lex_state_t;

#endif
