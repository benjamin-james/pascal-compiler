/* -*- C -*-
 *
 * standard_type.c
 *
 * Author: Benjamin T James
 */

#include "standard_type.h"

int standard_type(struct parser *p)
{
	int sync_set[] = {TOKEN_SEMICOLON, TOKEN_RPAREN};
	int expected[] = {TOKEN_INTEGER, TOKEN_REAL};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_INTEGER:
		ret = TYPE_INT;
		match(p, TOKEN_INTEGER, &ret);
		break;
	case TOKEN_REAL:
		ret = TYPE_REAL;
		match(p, TOKEN_REAL, &ret);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
