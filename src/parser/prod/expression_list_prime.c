/* -*- C -*-
 *
 * expression_list_prime.c
 *
 * Author: Benjamin T James
 */

#include "expression_list_prime.h"
#include "expression.h"
#include <string.h>
int expression_list_prime(struct parser *p, char **args)
{
	int sync_set[] = {TOKEN_RPAREN};
	int expected[] = {TOKEN_COMMA};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_COMMA:
		match(p, TOKEN_COMMA, &ret);
		int type = expression(p);
		char *arg = malloc(strlen(*args) + 2);
		sprintf(arg, "%s%c", *args, type);
		free(*args);
		*args = arg;
		ret = expression_list_prime(p, args);
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
