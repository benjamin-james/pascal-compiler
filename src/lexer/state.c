/* -*- C -*-
 *
 * state.c
 *
 * Author: Benjamin T James
 */

#include "state.h"
#include "util.h"

int resword_init(struct lex_state *st)
{
	int tok, attr;
	char *lexeme = malloc(st->buf.alloc);
	if (lexeme == NULL) {
		fprintf(stderr, "Could not allocate memory\n");
		return -1;
	}
	while (fscanf(st->res_word, "%s\t%d\t%d\n", lexeme, &tok, &attr) != EOF) {
		idres_add_rw(&st->rwords, lexeme, tok, attr);
	}
	free(lexeme);
	return 0;
}
int state_init(const char *source, const char *res_word,
	       int line_len, struct lex_state *st)
{
	if (init_buf(&st->buf, line_len) < 0) {
		return -1;
	}
	if (open_file(source, "list", &st->list) < 0) {
		return -1;
	}
	if (open_file(source, "tok", &st->token) < 0) {
		return -1;
	}
	if (open_file(source, "sym", &st->sym) < 0) {
		return -1;
	}
	st->res_word = fopen(res_word, "r");
	if (st->res_word == NULL) {
		fprintf(stderr, "Could not open file \"%s\"\n", res_word);
		return -1;
	}
	st->source = fopen(source, "r");
	if (st->source == NULL) {
		fprintf(stderr, "Could not open file \"%s\"\n", source);
		return -1;
	}
	st->rwords = NULL;
	st->ids = NULL;
	if (resword_init(st) < 0) {
		return -1;
	}
	st->machines = NULL;
	if (machine_init(&st->machines) < 0) {
		return -1;
	}
	return 0;
}

int state_cleanup(struct lex_state *s)
{
	free_buf(&s->buf);
	fclose(s->source);
	fclose(s->res_word);
	fclose(s->sym);
	fclose(s->list);
	fclose(s->token);
	idres_clean(&s->rwords);
	idres_clean(&s->ids);
	machine_clean(&s->machines);
	return 0;
}
