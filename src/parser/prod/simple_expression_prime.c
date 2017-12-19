/* -*- C -*-
 *
 * simple_expression_prime.c
 *
 * Author: Benjamin T James
 */

#include "simple_expression_prime.h"
#include "term.h"

int simple_expression_prime(struct parser *p, int type)
{
	int sync_set[] = {TOKEN_RELOP, TOKEN_DO, TOKEN_ELSE,
			  TOKEN_END, TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	int expected[] = {TOKEN_SIGN, TOKEN_ADDOP, TOKEN_RELOP,
			  TOKEN_DO, TOKEN_ELSE, TOKEN_END,
			  TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	struct token t_id;
	switch (p->token.type) {
	case TOKEN_SIGN: {/* used as addition/subtraction here */
		int term_type;
		t_id = p->token;
		match(p, TOKEN_SIGN, &ret);
		term_type = term(p);
		if (type != TYPE_ERR && term_type != TYPE_ERR) {
			if (type != term_type ||
			    (type != TYPE_INT
			     && type != TYPE_REAL) ||
			    (term_type != TYPE_INT
			     && term_type != TYPE_REAL)) {
				fprintf(p->syn_list, "SEMERR: Invalid operands to \"%c\": %s and %s\n",
					TOKEN_MINUS == t_id.val.attr ? '-' : '+',
					strtype(type),
					strtype(term_type));
				ret = TYPE_ERR;
			}
		}
		int endret = simple_expression_prime(p, term_type);
/*		err_propogate(, &ret);*/
		if (ret != TYPE_ERR) {
			ret = endret;
		}
		return ret;
		break;
	}
	case TOKEN_ADDOP: {/* OR */
		int term_type;
		match(p, TOKEN_ADDOP, &ret);

		term_type = term(p);
		if (type != TYPE_ERR && type != TYPE_BOOL && term_type != TYPE_ERR && term_type != TYPE_BOOL) {
			fprintf(p->syn_list, "SEMERR: Invalid operands to OR: %s and %s\n",
				strtype(type),
				strtype(term_type));
			ret = TYPE_ERR;
		}
		term_type = simple_expression_prime(p, term_type);
		if (ret != TYPE_ERR) {
			ret = term_type;
		}
		break;
	}
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
