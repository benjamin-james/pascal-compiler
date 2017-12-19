/* -*- C -*-
 *
 * sign.c
 *
 * Author: Benjamin T James
 */

#include "sign.h"

int sign(struct parser *p)
{
	int sync_set[] = {TOKEN_ID, TOKEN_NUM_INTEGER,
			  TOKEN_NUM_REAL, TOKEN_LPAREN,
			  TOKEN_NOT};
	int expected[] = {TOKEN_SIGN};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_SIGN:
		match(p, TOKEN_SIGN, &ret);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
