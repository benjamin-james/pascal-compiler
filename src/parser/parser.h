#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include "idres.h"
#include "defs.h"
#include "gb.h"
#include "types.h"

struct parser {
	struct idres *symbols;
	FILE* f_token;
	FILE* lex_list;
	FILE* syn_list;
	FILE* addr;
	char *buffer;
	unsigned tok_line, list_line;
	struct gb_state gbs;
	struct token token;
};

int parser_init(struct parser *p, const char* symbol_file, const char* token_file, const char* list_in, const char* addr_out, const char* list_out);

int parse(struct parser *p);

int match(struct parser *p, int tok, int *ret);

int sync(struct parser *p, int* sync_set, int sync_size);
void expected_found(struct parser *p, int *expected, int size);
int parser_cleanup(struct parser *p);

char* parser_get_tok_lex(struct parser *p,
			 struct token t_id);

#endif
