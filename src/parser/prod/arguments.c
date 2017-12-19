/* -*- C -*-
 *
 * arguments.c
 *
 * Author: Benjamin T James
 */

#include "arguments.h"
#include "parameter_list.h"

int arguments(struct parser *p, char **args)
{
	int sync_set[] = {TOKEN_COLON};
	int expected[] = {TOKEN_COLON, TOKEN_LPAREN};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_LPAREN:
		match(p, TOKEN_LPAREN, &ret);
		err_propogate(parameter_list(p, args), &ret);
		match(p, TOKEN_RPAREN, &ret);
		break;
	case TOKEN_COLON:
		*args = calloc(1, 1);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
