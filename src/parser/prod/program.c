/* -*- C -*-
 *
 * program.c
 *
 * Author: Benjamin T James
 */

#include "program.h"
#include "identifier_list.h"
#include "declarations.h"
#include "subprogram_declarations.h"
#include "compound_statement.h"

int program(struct parser *p)
{
	int expected = TOKEN_PROGRAM;
	struct token t_id;
	char *lexeme = NULL;
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_PROGRAM:
		match(p, TOKEN_PROGRAM, &ret);
		t_id = p->token;
		match(p, TOKEN_ID, &ret);
		lexeme = parser_get_tok_lex(p, t_id);
		if (lexeme != NULL) {
			if (check_add_green(&p->gbs, lexeme, TYPE_PGMNAME) == -1) {
				fprintf(p->syn_list, "SEMERR: Scope \"%s\" already exists\n", lexeme);
				ret = TYPE_ERR;
			}
		}

		match(p, TOKEN_LPAREN, &ret);
		err_propogate(identifier_list(p), &ret);
		match(p, TOKEN_RPAREN, &ret);
		match(p, TOKEN_SEMICOLON, &ret);
		err_propogate(declarations(p), &ret);
		err_propogate(subprogram_declarations(p), &ret);
		err_propogate(compound_statement(p), &ret);
		match(p, TOKEN_PERIOD, &ret);
		if (end_green(&p->gbs) == -1) {
			fprintf(p->syn_list, "SEMERR: Scope for \"program\" ended but never started\n");
			ret = TYPE_ERR;
		}
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, &expected, 1);
		sync(p, NULL, 0);
	}
	return ret;
}
