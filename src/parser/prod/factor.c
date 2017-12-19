/* -*- C -*-
 *
 * factor.c
 *
 * Author: Benjamin T James
 */

#include "factor.h"
#include "expression.h"
#include "factor_prime.h"

int factor(struct parser *p)
{
	int sync_set[] = {TOKEN_MULOP, TOKEN_ADDOP, TOKEN_SIGN, TOKEN_RELOP,
			  TOKEN_DO, TOKEN_ELSE, TOKEN_END,
			  TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	int expected[] = {TOKEN_ID, TOKEN_NUM_INTEGER,
			  TOKEN_NUM_REAL, TOKEN_LPAREN,
			  TOKEN_NOT};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_ID: {
		struct token t_id = p->token;
		char *lexeme = parser_get_tok_lex(p, t_id);
		int type;
		match(p, TOKEN_ID, &ret);
		type = factor_prime(p, lexeme);
		if (ret != TYPE_ERR) {
			ret = type;
		}
		break;
	}
	case TOKEN_NUM_INTEGER:
		match(p, TOKEN_NUM_INTEGER, &ret);
		ret = TYPE_INT;
		break;
	case TOKEN_NUM_REAL:
		match(p, TOKEN_NUM_REAL, &ret);
		ret = TYPE_REAL;
		break;
	case TOKEN_LPAREN: {
		int type;
		match(p, TOKEN_LPAREN, &ret);
		type = expression(p);
		match(p, TOKEN_RPAREN, &ret);
		if (ret != TYPE_ERR) {
			ret = type;
		}
		break;
	}
	case TOKEN_NOT: {
		int type;
		match(p, TOKEN_NOT, &ret);
		type = factor(p);
		if (type != TYPE_ERR && type != TYPE_BOOL) {
			fprintf(p->syn_list, "SEMERR: Invalid operand to \"not\": %s\n",
				strtype(type));
			type = TYPE_ERR;
		}
		if (ret != TYPE_ERR) {
			ret = type;
		}
		break;
	}
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
