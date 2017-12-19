/* -*- C -*-
 *
 * expression_list.c
 *
 * Author: Benjamin T James
 */

#include "expression_list.h"
#include "expression_list_prime.h"
#include "expression.h"
int expression_list(struct parser *p, char **args)
{
	int sync_set[] = {TOKEN_RPAREN};
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
	case TOKEN_SIGN: {
		int type = expression(p);
		char *arg = malloc(2);
		sprintf(arg, "%c", type);
		*args = arg;
		ret = expression_list_prime(p, args);
		break;
	}
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
