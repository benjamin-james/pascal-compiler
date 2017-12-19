/* -*- C -*-
 *
 * type.c
 *
 * Author: Benjamin T James
 */

#include "type.h"
#include "standard_type.h"
int type(struct parser *p, int *width)
{
	int sync_set[] = {TOKEN_SEMICOLON, TOKEN_RPAREN};
	int expected[] = {TOKEN_INTEGER, TOKEN_REAL, TOKEN_ARRAY};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int a_begin, a_end, a_len;
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_INTEGER:
	case TOKEN_REAL:
		ret = standard_type(p);
		*width = type_width(ret);
		break;
	case TOKEN_ARRAY:
		match(p, TOKEN_ARRAY, &ret);
		match(p, TOKEN_LBRACKET, &ret);
		a_begin = p->token.val.attr;
		match(p, TOKEN_NUM_INTEGER, &ret);
		if (a_begin != 1) {
			fprintf(p->syn_list, "SEMERR: Arrays must start at 1\n");
			ret = TYPE_ERR;
		}
		match(p, TOKEN_ELLIPSIS, &ret);
		a_end = p->token.val.attr;
		match(p, TOKEN_NUM_INTEGER, &ret);
		a_len = a_end - a_begin + 1;
		if (a_end < a_begin) {
			fprintf(p->syn_list, "SEMERR: Must have a positive length array (Length is %d)\n", a_len);
			ret = TYPE_ERR;
		}
		match(p, TOKEN_RBRACKET, &ret);
		match(p, TOKEN_OF, &ret);
		ret = standard_type(p);
		*width = type_width(ret) * a_len;
		ret = SCALAR_TO_ARRAY(ret);
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
