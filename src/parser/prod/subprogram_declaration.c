/* -*- C -*-
 *
 * subprogram_declaration.c
 *
 * Author: Benjamin T James
 */

#include "subprogram_declaration.h"
#include "subprogram_head.h"
#include "declarations.h"
#include "subprogram_declarations.h"
#include "compound_statement.h"

int subprogram_declaration(struct parser *p)
{
	int sync_set[] = {TOKEN_SEMICOLON};
	int expected[] = {TOKEN_FUNCTION};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_FUNCTION:
		err_propogate(subprogram_head(p), &ret);
		err_propogate(declarations(p), &ret);
		err_propogate(subprogram_declarations(p), &ret);
		err_propogate(compound_statement(p), &ret);
		/* if (check_green_set(&p->gbs) == -1) { */
		/* 	fprintf(p->syn_list, "SEMERR: Function return value not set\n"); */
		/* 	ret = -1; */
		/* } */
		if (end_green(&p->gbs) == -1) {
			fprintf(p->syn_list, "SEMERR: Scope for function ended but never started\n");
			ret = TYPE_ERR;
		}
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
