/* -*- C -*-
 *
 * statement_list.c
 *
 * Author: Benjamin T James
 */

#include "statement_list.h"
#include "statement_list_prime.h"
#include "statement.h"
int statement_list(struct parser *p)
{
	int sync_set[] = {TOKEN_END};
	int expected[] = {TOKEN_BEGIN, TOKEN_ID, TOKEN_IF, TOKEN_WHILE};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_BEGIN:
	case TOKEN_ID:
	case TOKEN_IF:
	case TOKEN_WHILE:
		err_propogate(statement(p), &ret);
		err_propogate(statement_list_prime(p), &ret);
		break;
	case TOKEN_END:
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
