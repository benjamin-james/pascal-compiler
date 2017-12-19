/* -*- C -*-
 *
 * compound_statement.c
 *
 * Author: Benjamin T James
 */

#include "compound_statement.h"
#include "optional_statements.h"

int compound_statement(struct parser *p)
{
	int sync_set[] = {TOKEN_PERIOD, TOKEN_SEMICOLON, TOKEN_END, TOKEN_ELSE};
	int expected[] = {TOKEN_BEGIN};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_BEGIN:
		match(p, TOKEN_BEGIN, &ret);
		err_propogate(optional_statements(p), &ret);
		match(p, TOKEN_END, &ret);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
