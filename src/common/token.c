/* -*- C -*-
 *
 * token.c
 *
 * Author: Benjamin T James
 */

#include "token.h"
#include "defs.h"

int token_id(struct token *t, char *ptr)
{
	t->is_id = 1;
	t->type = TOKEN_ID;
	t->val.ptr = ptr;
	return 0;
}

int token_add(struct token *t, int type, int attr)
{
	t->is_id = 0;
	t->type = type;
	t->val.attr = attr;
	return 0;
}

int token_println(FILE* f, int line, const char *lexeme, struct token t)
{
	if (t.type & NOPRINT) {
		return 0;
	} else if (t.is_id) {
		return fprintf(f, "%d\t%s\t  %d\t%p\n",
			       line, lexeme, t.type, t.val.ptr);
	} else {
		return fprintf(f, "%d\t%s\t  %d\t%d\n",
			       line, lexeme, t.type, t.val.attr);
	}
}


const char *token2str(int token)
{
	switch (token) {
	case  TOKEN_ADDOP: {
		return  "ADDOP";
	}
	case  TOKEN_ARRAY:        return  "array";
	case  TOKEN_ASSIGN:       return  ":=";
	case  TOKEN_BEGIN:        return  "begin";
	case  TOKEN_COLON:        return  ":";
	case  TOKEN_COMMA:        return  ",";
	case  TOKEN_DO:           return  "do";
	case  TOKEN_ELLIPSIS:     return  "..";
	case  TOKEN_ELSE:         return  "else";
	case  TOKEN_END:          return  "end";
	case  TOKEN_EOF:          return  "EOF";
	case  TOKEN_FUNCTION:     return  "function";
	case  TOKEN_ID:           return  "identifier";
	case  TOKEN_IF:           return  "if";
	case  TOKEN_INTEGER:      return  "integer";
	case  TOKEN_LBRACKET:     return  "[";
	case  TOKEN_LPAREN:       return  "(";
	case  TOKEN_MULOP: {
		return  "MULOP";
	}
	case  TOKEN_NOT:          return  "not";
	case  TOKEN_NUM_INTEGER:  return  "NUM_INTEGER";
	case  TOKEN_NUM_REAL:     return  "NUM_REAL";
	case  TOKEN_OF:           return  "of";
	case  TOKEN_PERIOD:       return  ".";
	case  TOKEN_PROGRAM:      return  "program";
	case  TOKEN_RBRACKET:     return  "]";
	case  TOKEN_REAL:         return  "real";
	case  TOKEN_RELOP: {
		return  "RELOP";
	}
	case  TOKEN_RPAREN:       return  ")";
	case  TOKEN_SEMICOLON:    return  ";";
	case  TOKEN_SIGN:         return  "+ or -";
	case  TOKEN_THEN:         return  "then";
	case  TOKEN_VAR:          return  "var";
	case  TOKEN_WHILE:        return  "while";
	case  LEXERR:             return  "LEXERR";
	}
	return "UNKNOWN";
}

const char* relop2str(int attr)
{
	switch (attr) {
	case TOKEN_LT:  return "<";
	case TOKEN_LEQ: return "<=";
	case TOKEN_EQ:  return "=";
	case TOKEN_NEQ: return "<>";
	case TOKEN_GT:  return ">";
	case TOKEN_GEQ: return ">=";
	default:        return "Unknown relop";
	}
}
