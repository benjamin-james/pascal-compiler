/* -*- C -*-
 *
 * term_prime.c
 *
 * Author: Benjamin T James
 */

#include "term_prime.h"
#include "factor.h"

int mulop(struct parser *p, int t1, int op, int t2, int *p_ret);
int term_prime(struct parser *p, int type)
{
	int sync_set[] = {TOKEN_SIGN, TOKEN_ADDOP, TOKEN_RELOP,
			  TOKEN_DO, TOKEN_ELSE, TOKEN_END,
			  TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	int expected[] = {TOKEN_MULOP, TOKEN_SIGN, TOKEN_ADDOP, TOKEN_RELOP,
			  TOKEN_DO, TOKEN_ELSE, TOKEN_END,
			  TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_MULOP: {
		int ftype, op;
		op = p->token.val.attr;
		match(p, TOKEN_MULOP, &ret);
		ftype = factor(p);
		mulop(p, type, op, ftype, &ret);
		ret = term_prime(p, ret);
		break;
	}
	case TOKEN_SIGN: /* used as addition/subtraction here */
	case TOKEN_ADDOP:
	case TOKEN_RELOP:
	case TOKEN_DO:
	case TOKEN_ELSE:
	case TOKEN_END:
	case TOKEN_THEN:
	case TOKEN_RPAREN:
	case TOKEN_RBRACKET:
	case TOKEN_COMMA:
	case TOKEN_SEMICOLON:
		ret = type;
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}

int mulop(struct parser *p, int t1, int op, int t2, int *p_ret)
{
	if (t1 == TYPE_ERR || t2 == TYPE_ERR) {
		*p_ret = TYPE_ERR;
	} else if (op == TOKEN_AND) {
		if (t1 != TYPE_BOOL || t2 != TYPE_BOOL) {
			fprintf(p->syn_list, "SEMERR: Invalid operands to \"and\": %s and %s\n",
				strtype(t1),
				strtype(t2));
	    		*p_ret = TYPE_ERR;
		} else {
			*p_ret = TYPE_BOOL;
		}
	} else if (op == TOKEN_TIMES) {
		if (t1 == TYPE_INT && t2 == TYPE_INT) {
			*p_ret = TYPE_INT;
		} else if (t1 == TYPE_REAL && t2 == TYPE_REAL) {
			*p_ret = TYPE_REAL;
		} else {
			fprintf(p->syn_list, "SEMERR: Invalid operations to \"*\": %s and %s\n",
				strtype(t1),
				strtype(t2));
			*p_ret = TYPE_ERR;
		}
	} else if (op == TOKEN_RDIV) {
		if (t1 != TYPE_REAL || t2 != TYPE_REAL) {
			fprintf(p->syn_list, "SEMERR: Invalid operands to \"/\": %s and %s\n",
				strtype(t1),
				strtype(t2));
	    		*p_ret = TYPE_ERR;
		} else {
			*p_ret = TYPE_REAL;
		}
	} else if (op == TOKEN_IDIV) {
		if (t1 != TYPE_INT || t2 != TYPE_INT) {
			fprintf(p->syn_list, "SEMERR: Invalid operands to \"div\": %s and %s\n",
				strtype(t1),
				strtype(t2));
	    		*p_ret = TYPE_ERR;
		} else {
			*p_ret = TYPE_INT;
		}
	} else if (op == TOKEN_MOD) {
		if (t1 != TYPE_INT || t2 != TYPE_INT) {
			fprintf(p->syn_list, "SEMERR: Invalid operands to \"mod\": %s and %s\n",
				strtype(t1),
				strtype(t2));
	    		*p_ret = TYPE_ERR;
		} else {
			*p_ret = TYPE_INT;
		}
	} else {
		fprintf(stderr, "unknown operation %d\n", op);
	}
	return *p_ret;
}
