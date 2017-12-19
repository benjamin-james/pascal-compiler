/* -*- C -*-
 *
 * declarations.c
 *
 * Author: Benjamin T James
 */

#include "declarations.h"
#include "type.h"
int declarations(struct parser *p)
{
	int sync_set[] = {TOKEN_BEGIN, TOKEN_FUNCTION};
	int expected[] = {TOKEN_BEGIN, TOKEN_FUNCTION, TOKEN_VAR};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	struct token t_id;
	int ret = TYPE_VOID;
	char *lexeme = NULL;
	switch (p->token.type) {
	case TOKEN_VAR:
		match(p, TOKEN_VAR, &ret);
	        t_id = p->token;
		match(p, TOKEN_ID, &ret);
		match(p, TOKEN_COLON, &ret);
		{
			int offset, var_type, width;
			lexeme = parser_get_tok_lex(p, t_id);
			var_type = type(p, &width);
			err_propogate(var_type, &ret);
			get_offset(&p->gbs, &offset);
			int bret = check_add_blue(&p->gbs, lexeme, var_type, offset);
			if (lexeme != NULL) {
				if (bret == -1) {
					fprintf(p->syn_list,
						"SEMERR: Identifier \"%s\" already declared\n",
						lexeme);
					ret = TYPE_ERR;
				} else if (bret < 0) {
					ret = TYPE_ERR;
				} else {
					fprintf(p->addr, "%s\t%d\t%s\n", lexeme, offset, strtype(var_type));
				}
			}
			set_offset(&p->gbs, offset + width);
		}
		match(p, TOKEN_SEMICOLON, &ret);
		err_propogate(declarations(p), &ret);
		break;
	case TOKEN_BEGIN:
	case TOKEN_FUNCTION:
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
