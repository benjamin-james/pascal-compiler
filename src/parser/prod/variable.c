/* -*- C -*-
 *
 * variable.c
 *
 * Author: Benjamin T James
 */

#include "variable.h"
#include "variable_prime.h"

int variable(struct parser *p)
{
	int sync_set[] = {TOKEN_ASSIGN};
	int expected[] = {TOKEN_ID};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID, type = TYPE_ERR;
	struct token t_id;
	char *lexeme = NULL;
	switch (p->token.type) {
	case TOKEN_ID:
		t_id = p->token;
		match(p, TOKEN_ID, &ret);
		lexeme = parser_get_tok_lex(p, t_id);
		if (lexeme != NULL &&
		    gettype(&p->gbs, lexeme, &type) < 0) {
			fprintf(p->syn_list, "SEMERR: Variable \"%s\" was not declared in this scope\n", lexeme);
			ret = TYPE_ERR;
		}
		if (FUNC_TO_SCALAR(type) != TYPE_ERR) {
			/* function return value */
			type = FUNC_TO_SCALAR(type);
		}
		/* synthesized variable type */
		ret = variable_prime(p, type);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
