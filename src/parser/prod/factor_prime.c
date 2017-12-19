/* -*- C -*-
 *
 * factor_prime.c
 *
 * Author: Benjamin T James
 */

#include "factor_prime.h"
#include "expression.h"
#include "expression_list.h"
#include <string.h>
int fp_array(struct parser *p, char *id_lex);
int fp_function(struct parser *p, char *id_lex, int parens);
int fp_id(struct parser *p, char *id_lex);

int factor_prime(struct parser *p, char *id_lex)
{
	int sync_set[] = {TOKEN_MULOP, TOKEN_ADDOP, TOKEN_SIGN, TOKEN_RELOP,
			  TOKEN_DO, TOKEN_ELSE, TOKEN_END,
			  TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	int expected[] = {TOKEN_LBRACKET, TOKEN_LPAREN, TOKEN_MULOP,
			  TOKEN_ADDOP, TOKEN_SIGN, TOKEN_RELOP,
			  TOKEN_DO, TOKEN_ELSE, TOKEN_END,
			  TOKEN_THEN, TOKEN_RPAREN,
			  TOKEN_RBRACKET, TOKEN_COMMA,
			  TOKEN_SEMICOLON};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	/*
	  		if (lexeme == NULL &&  < 0) {
			fprintf(p->syn_list, "SEMERR: Symbol \"%s\" was not declared in this scope\n", lexeme);
			ret = TYPE_ERR;
		}

	 */
	switch (p->token.type) {
	case TOKEN_LBRACKET: {
		ret = fp_array(p, id_lex);
		break;
	}
	case TOKEN_LPAREN: { /* turn FPINT -> INT */
		ret = fp_function(p, id_lex, 1);
		break;
	}
	case TOKEN_MULOP:
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
	case TOKEN_SEMICOLON: /* ID */ {
		ret = fp_id(p, id_lex);
		break;
	}
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}

int fp_array(struct parser *p, char *id_lex)
{
	int ret = TYPE_VOID;
	int expr_type, atype, scalar_type, type;
	if (gettype(&p->gbs, id_lex, &atype) < 0) {
		fprintf(p->syn_list, "SEMERR: Variable \"%s\" was not declared in this scope\n", id_lex);
		ret = TYPE_ERR;
	}
	scalar_type = ARRAY_TO_SCALAR(atype);
	if (scalar_type == TYPE_ERR) {
		fprintf(p->syn_list, "SEMERR: Attempted indexing of non-array type %s\n", strtype(atype));
		ret = TYPE_ERR;
	} else {
		type = scalar_type;
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
		ret = type;
	}
	return ret;
}

int fp_function(struct parser *p, char *id_lex, int parens)
{
	char *args = NULL;
	int ret = TYPE_VOID;
	if (parens) {
		match(p, TOKEN_LPAREN, &ret);
	}
	err_propogate(expression_list(p, &args), &ret);
	if (parens) {
		match(p, TOKEN_RPAREN, &ret);
	}
	struct gb *gbret = NULL;
	find_green(p->gbs.cur_eye, id_lex, &gbret);
	if (gbret == NULL) {
		fprintf(p->syn_list, "SEMERR: Function \"%s\" not declared in this scope\n", id_lex);
		if (args) {
			free(args);
		}
		ret = TYPE_ERR;
		return ret;
	}
	char *fargs = gbret->n.g.arglist;
	ret = FUNC_TO_SCALAR(gbret->n.g.type);
	if (args == NULL || strcmp(args, fargs)) {
		if (args == NULL) {
			args = calloc(0,0);
		}
		char *exp = param2str(fargs);
		char *got = param2str(args);
		fprintf(p->syn_list, "SEMERR: Function args for \"%s\" do not match: expected (%s) but got (%s)\n",
			id_lex,
			exp,
			got);
		ret = TYPE_ERR;
		free(exp);
		free(got);
	}
	if (args) {
		free(args);
	}
	return ret;
}

int fp_id(struct parser *p, char *id_lex)
{
	int type = TYPE_ERR;
	if (id_lex != NULL &&
	    gettype(&p->gbs, id_lex, &type) < 0) {
		fprintf(p->syn_list, "SEMERR: Variable \"%s\" was not declared in this scope\n", id_lex);
		type = TYPE_ERR;
	}
	if (type == TYPE_FINT || type == TYPE_FREAL) {

		struct gb *gbret = NULL;
		find_green(p->gbs.cur_eye, id_lex, &gbret);
		if (gbret == NULL) {
			fprintf(p->syn_list, "SEMERR: Function \"%s\" not declared in this scope\n", id_lex);
			return TYPE_ERR;
		}
		char *fargs = gbret->n.g.arglist;
		char *exp = param2str(fargs);

		fprintf(p->syn_list, "SEMERR: Function args for \"%s\" do not match: expected (%s) but got ()\n",
			id_lex,
			exp);
		free(exp);
		return TYPE_ERR;
	}
	return type;
}
