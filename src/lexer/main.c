/* -*- C -*-
 *
 * main.c
 *
 * Author: Benjamin T James
 */

#include "state.h"
#include "defs.h"
#include "lexerr.h"

int handle_line(struct lex_state *s, int line_no)
{
	char *lexeme = NULL;
	struct machine state;
	state.f = s->buf.buf;
	state.b = state.f;
	state.tok.type = 0;
	if (s->buf.err == LEXERR_LINE_TOO_LONG) {
		print_error(s->list, s->buf.err, s->buf.buf);
	}
	while (state.tok.type != TOKEN_NEWLINE) {
		int ret = machine_iter(s, &state, &lexeme);
		if (ret < 0) {
			fprintf(stderr, "Machine not found\n");
			return -1;
		}
		if (state.tok.type == LEXERR) {
			print_error(s->list, state.tok.val.attr, lexeme);
		}
		token_println(s->token, line_no, lexeme, state.tok);
		free(lexeme);
		lexeme = NULL;
	}
	return 0;
}

int main(int argc, char **argv)
{
	struct lex_state s;
	struct token tok_eof;
	int line = 1;
	if (argc != 3) {
		fprintf(stderr, "Usage: %s source reservedWordFile\n", *argv);
		return -1;
	}

	if (state_init(argv[1], argv[2], LINELEN, &s) < 0) {
		return -1;
	}

	while (read_line(&s.buf, s.source) == 0) {
		fprintf(s.list, "%d\t%s", line, s.buf.buf);

		handle_line(&s, line);

		line++;
	}
	token_add(&tok_eof, TOKEN_EOF, 0);
	token_println(s.token, line, "EOF", tok_eof);
	idres_print(s.sym, &s.ids);
	state_cleanup(&s);
	return 0;
}
