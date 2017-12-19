/* -*- C -*-
 *
 * expression_prime.c
 *
 * Author: Benjamin T James
 */

#include "expression_prime.h"
#include "simple_expression.h"

int expression_prime(struct parser *p, int type)
{
	int sync_set[] = {TOKEN_DO, TOKEN_ELSE, TOKEN_END,
			  TOKEN_THEN, TOKEN_RPAREN, TOKEN_RBRACKET,
			  TOKEN_COMMA, TOKEN_SEMICOLON};
	int expected[] = {TOKEN_RELOP, TOKEN_DO, TOKEN_ELSE,
			  TOKEN_END, TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = type;
	switch (p->token.type) {
	case TOKEN_RELOP: {
		int type2;
		struct token t_relop = p->token;
		match(p, TOKEN_RELOP, &ret);
		type2 = simple_expression(p);
		if (type != TYPE_ERR && type2 != TYPE_ERR) {
			if (type != type2) {
				fprintf(p->syn_list, "SEMERR: Invalid operands to %s: %s and %s\n",
					relop2str(t_relop.val.attr),
					strtype(type),
					strtype(type2));
				ret = TYPE_ERR;
			} else {
				ret = TYPE_BOOL;
			}
		} else {
			ret = TYPE_ERR;
		}
		break;
	}
	case TOKEN_DO:
	case TOKEN_ELSE:
	case TOKEN_END:
	case TOKEN_THEN:
	case TOKEN_RPAREN:
	case TOKEN_RBRACKET:
	case TOKEN_COMMA:
	case TOKEN_SEMICOLON:
		ret = type;
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
