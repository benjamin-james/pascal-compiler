/* -*- C -*-
 *
 * subprogram_declarations.c
 *
 * Author: Benjamin T James
 */
#include "subprogram_declaration.h"
#include "subprogram_declarations.h"

int subprogram_declarations(struct parser *p)
{
	int sync_set[] = {TOKEN_BEGIN};
	int expected[] = {TOKEN_BEGIN, TOKEN_FUNCTION};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_BEGIN:
		break;
	case TOKEN_FUNCTION:
		err_propogate(subprogram_declaration(p), &ret);
		match(p, TOKEN_SEMICOLON, &ret);
		err_propogate(subprogram_declarations(p), &ret);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
