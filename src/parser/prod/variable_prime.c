/* -*- C -*-
 *
 * variable_prime.c
 *
 * Author: Benjamin T James
 */

#include "variable_prime.h"
#include "expression.h"
int variable_prime(struct parser *p, int type)
{
	int sync_set[] = {TOKEN_ASSIGN};
	int expected[] = {TOKEN_LBRACKET, TOKEN_ASSIGN};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_LBRACKET: {/* type array */
		int expr_type, scalar_type, rtype;
		scalar_type = ARRAY_TO_SCALAR(type);
		if (scalar_type == TYPE_ERR) {
			fprintf(p->syn_list, "SEMERR: Attempted indexing of non-array type %s\n", strtype(type));
			rtype = TYPE_ERR;
		} else {
			rtype = scalar_type;
		}

		match(p, TOKEN_LBRACKET, &ret);
		expr_type = expression(p);
		if (expr_type != TYPE_ERR && expr_type != TYPE_INT) {
			fprintf(p->syn_list, "SEMERR: Array index must be an integer (found %s)\n",
				strtype(expr_type));
			ret = TYPE_ERR;
		}
		match(p, TOKEN_RBRACKET, &ret);
		if (ret != TYPE_ERR) {
			ret = rtype;
		}
		break;
	}
	case TOKEN_ASSIGN:
		ret = type;
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;

}
