#include "defs.h"
#include "parser.h"
#include "tokenizer.h"
#include "./prod/program.h"

int parse(struct parser *p)
{
	next_token(p);
	program(p);
	return match(p, TOKEN_EOF, NULL);
}

int match(struct parser *p, int tok, int *ret)
{
	if (p->token.type == tok) {
		if (tok == TOKEN_EOF) {
			return 0;
		} else {
			printf("Matched token %s\n", token2str(tok));
			next_token(p);
			return 0;
		}
	} else {
		expected_found(p, &tok, 1);
		next_token(p);
		if (ret) {
			*ret = -1;
		}
		return -1;
	}
}
int parser_init(struct parser *p, const char* symbol_file, const char* token_file, const char* list_in, const char* addr_out, const char* list_out)
{
	p->symbols = NULL;
	if (idres_read(symbol_file, &p->symbols) == -1) {
		return -1;
	}
	p->f_token = fopen(token_file, "r");
	if (p->f_token == NULL) {
		fprintf(stderr, "Could not open file \"%s\"\n", token_file);
		return -1;
	}
	p->addr = fopen(addr_out, "w");
	if (p->addr == NULL) {
		fprintf(stderr, "Could not open file \"%s\"\n", addr_out);
		return -1;
	}
	p->buffer = malloc(LINELEN+1);
	if (p->buffer == NULL) {
		fprintf(stderr, "Could allocate memory\n");
		return -1;
	}
	p->lex_list = fopen(list_in, "r");
	if (p->lex_list == NULL) {
		fprintf(stderr, "Could not open file \"%s\"\n", list_in);
		return -1;
	}
	p->syn_list = fopen(list_out, "w");
	if (p->syn_list == NULL) {
		fprintf(stderr, "Could not open file \"%s\"\n", list_out);
		return -1;
	}
	p->tok_line = 0;
	p->list_line = 0;
	return gb_state_init(&p->gbs);
}

void expected_found(struct parser *p, int* expected, int size)
{
	int i;
	fprintf(p->syn_list, "SYNERR: Expected ");
	if (size > 1) {
		fprintf(p->syn_list, "one of ");
	}
	for (i = 0; i < size - 1; i++) {
		fprintf(p->syn_list, "\"%s\"", token2str(expected[i]));
		if (size > 2) {
			fputc(',', p->syn_list);
		}
		fputc(' ', p->syn_list);
	}
	if (size > 1) {
		fprintf(p->syn_list, "or ");
	}
	fprintf(p->syn_list, "\"%s\" ", token2str(expected[size-1]));
	if (p->token.is_id) {
		struct idres *ret = NULL;
		idres_lookup(&p->symbols, p->token.val.ptr, &ret);
		if (ret == NULL) {
			fprintf(stderr, "symbol not found\n");
			exit(1);
		}
		fprintf(p->syn_list, "but found identifier \"%s\"\n", ret->lexeme);
	} else {
		fprintf(p->syn_list, "but found \"%s\"\n", token2str(p->token.type));
	}
}

int sync(struct parser *p, int* sync_set, int sync_size)
{
	int i, found = 0;
	while (!found) {
		for (i = 0; i < sync_size; i++) {
			if (p->token.type == sync_set[i]) {
				return 0;
			}
		}
		if (p->token.type == TOKEN_EOF) {
			return 0;
		}
		next_token(p);
	}
	return 0;
}

int parser_cleanup(struct parser *p)
{
	int c;
	while ((c = fgetc(p->lex_list)) != EOF) {
		fputc(c, p->syn_list);
	}
	free(p->buffer);
	/* gb_print_all(p->gbs.cur_eye, p->addr, 4); */
	gb_state_free(&p->gbs);
	fclose(p->addr);
	fclose(p->f_token);
	fclose(p->lex_list);
	fclose(p->syn_list);
	idres_clean(&p->symbols);

	return 0;
}


char* parser_get_tok_lex(struct parser *p,
			 struct token t_id)
{
	struct idres *ret = NULL;
	if (!t_id.is_id
	    || idres_lookup(&p->symbols, t_id.val.ptr, &ret) == -1
	    || ret == NULL) {
		fprintf(stderr, "INTERNAL ERROR: symbol table"
			" does not match token: \"%s\"\n", token2str(t_id.type));
		exit(EXIT_FAILURE);
	}
	return ret->lexeme;
}
