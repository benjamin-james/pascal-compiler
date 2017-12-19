/* -*- C -*-
 *
 * token.h
 *
 * Author: Benjamin T James
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

union tok_val {
	int attr;
	void *ptr;
};

struct token {
	int type;
/*	char *lex; */
	unsigned is_id : 1;
	union tok_val val;
};

int token_id(struct token *t, char *ptr);
int token_add(struct token *t, int type, int attr);
int token_println(FILE *f, int line, const char *lexeme, struct token t);
const char* token2str(int token);
const char* relop2str(int attr);
#endif
