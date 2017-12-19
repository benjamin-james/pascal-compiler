/* -*- C -*-
 *
 * subprogram_head.c
 *
 * Author: Benjamin T James
 */

#include "subprogram_head.h"
#include "arguments.h"
#include "standard_type.h"

int subprogram_head(struct parser *p)
{
	int sync_set[] = {TOKEN_BEGIN, TOKEN_FUNCTION, TOKEN_VAR};
	int expected[] = {TOKEN_FUNCTION};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	char *lexeme = NULL;
	char *args = NULL;
	struct token t_id;
	int ret_type = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_FUNCTION:
		match(p, TOKEN_FUNCTION, &ret);
		t_id = p->token;
		match(p, TOKEN_ID, &ret);
		lexeme = parser_get_tok_lex(p, t_id);
		if (lexeme != NULL) {
			if (check_add_green(&p->gbs, lexeme, TYPE_PLACEHOLDER) == -1) {
				fprintf(p->syn_list, "SEMERR: Scope \"%s\" already exists\n", lexeme);
				ret = TYPE_ERR;
			}
		}
		err_propogate(arguments(p, &args), &ret);
		match(p, TOKEN_COLON, &ret);
		ret_type = standard_type(p);
		err_propogate(ret_type, &ret);
		ret_type = SCALAR_TO_FUNC(ret_type);
		set_peek_args_type(&p->gbs, ret_type, args);
		free(args);
		match(p, TOKEN_SEMICOLON, &ret);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
