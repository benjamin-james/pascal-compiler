/* -*- C -*-
 *
 * identifier_list_prime.c
 *
 * Author: Benjamin T James
 */

#include "identifier_list_prime.h"

int identifier_list_prime(struct parser *p)
{
	int sync_set[] = {TOKEN_RPAREN};
	int expected[] = {TOKEN_COMMA, TOKEN_RPAREN};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	struct token t_id;
	int offset, ret = TYPE_VOID;
	char *lexeme;
	switch (p->token.type) {
	case TOKEN_COMMA:
		match(p, TOKEN_COMMA, &ret);
		t_id = p->token;
		match(p, TOKEN_ID, &ret);
		{
			lexeme = parser_get_tok_lex(p, t_id);
			get_offset(&p->gbs, &offset);
			int bret = check_add_blue(&p->gbs, lexeme, TYPE_PGMPARAM, offset);
			if (bret == -1) {
				fprintf(p->syn_list,
					"SEMERR: Identifier \"%s\" already declared\n",
					lexeme);
				ret = TYPE_ERR;
			} else if (bret < 0) {
				/* err already reported */
				ret = TYPE_ERR;
			}
			offset += type_width(TYPE_PGMPARAM);
			set_offset(&p->gbs, offset);
		}
		err_propogate(identifier_list_prime(p), &ret);
		break;
	case TOKEN_RPAREN:
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
