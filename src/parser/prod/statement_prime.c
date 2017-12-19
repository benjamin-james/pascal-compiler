/* -*- C -*-
 *
 * statement_prime.c
 *
 * Author: Benjamin T James
 */

#include "statement_prime.h"
#include "statement.h"

int statement_prime(struct parser *p)
{
	int sync_set[] = {TOKEN_ELSE, TOKEN_END, TOKEN_SEMICOLON};
	int expected[] = {TOKEN_ELSE, TOKEN_END, TOKEN_SEMICOLON};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_ELSE:
		match(p, TOKEN_ELSE, &ret);
		statement(p);
	case TOKEN_SEMICOLON:
	case TOKEN_END:
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
