/* -*- C -*-
 *
 * expression.c
 *
 * Author: Benjamin T James
 */

#include "expression.h"
#include "expression_prime.h"
#include "simple_expression.h"

int expression(struct parser *p)
{
	int sync_set[] = {TOKEN_DO, TOKEN_ELSE, TOKEN_END,
			  TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	int expected[] = {TOKEN_ID, TOKEN_NUM_INTEGER,
			  TOKEN_NUM_REAL, TOKEN_LPAREN,
			  TOKEN_NOT, TOKEN_SIGN};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;

	switch (p->token.type) {
	case TOKEN_ID:
	case TOKEN_NUM_INTEGER:
	case TOKEN_NUM_REAL:
	case TOKEN_LPAREN:
	case TOKEN_NOT:
	case TOKEN_SIGN:
		ret = simple_expression(p);
		ret = expression_prime(p, ret);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
