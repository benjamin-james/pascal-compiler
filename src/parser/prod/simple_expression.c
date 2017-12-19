/* -*- C -*-
 *
 * simple_expression.c
 *
 * Author: Benjamin T James
 */

#include "simple_expression.h"
#include "term.h"
#include "simple_expression_prime.h"
#include "sign.h"

int simple_expression(struct parser *p)
{
	int sync_set[] = {TOKEN_RELOP, TOKEN_DO, TOKEN_ELSE,
			  TOKEN_END, TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	int expected[] = {TOKEN_ID, TOKEN_NUM_INTEGER,
			  TOKEN_NUM_REAL, TOKEN_LPAREN,
			  TOKEN_NOT, TOKEN_SIGN};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_SIGN: {
		int term_type;
		sign(p);
		term_type = term(p);
		err_propogate(term_type, &ret);
		if (term_type != TYPE_ERR && term_type != TYPE_INT && term_type != TYPE_REAL) {
			fprintf(p->syn_list, "Cannot apply a sign to type %s\n",
				strtype(ret));
			ret = TYPE_ERR;
		}
	        term_type = simple_expression_prime(p, term_type);
		if (ret != TYPE_ERR) {
			ret = term_type;
		}
		break;
	}
	case TOKEN_ID:
	case TOKEN_NUM_INTEGER:
	case TOKEN_NUM_REAL:
	case TOKEN_LPAREN:
	case TOKEN_NOT:
		ret = term(p);
		ret = simple_expression_prime(p, ret);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
