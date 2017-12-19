/* -*- C -*-
 *
 * statement.c
 *
 * Author: Benjamin T James
 */

#include "statement.h"
#include "compound_statement.h"
#include "variable.h"
#include "expression.h"
#include "statement_prime.h"

int statement(struct parser *p)
{
	int sync_set[] = {TOKEN_END, TOKEN_ELSE, TOKEN_SEMICOLON};
	int expected[] = {TOKEN_BEGIN, TOKEN_ID, TOKEN_IF, TOKEN_WHILE};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	int ret = TYPE_VOID;
	switch (p->token.type) {
	case TOKEN_BEGIN:
		err_propogate(compound_statement(p), &ret);
		break;
	case TOKEN_ID: {
		int var_type, expr_type;
		var_type = variable(p);
		err_propogate(var_type, &ret);
		match(p, TOKEN_ASSIGN, &ret);
		expr_type = expression(p);
		err_propogate(expr_type, &ret);
		if (var_type != expr_type &&
		    var_type != TYPE_ERR &&
		    expr_type != TYPE_ERR) {
			fprintf(p->syn_list, "SEMERR: Cannot assign type \"%s\" to variable of type \"%s\"\n", strtype(expr_type), strtype(var_type));
			ret = TYPE_ERR;
		}
		break;
	}
	case TOKEN_IF: {
		int expr_type;
		match(p, TOKEN_IF, &ret);
		expr_type = expression(p);
		err_propogate(expr_type, &ret);
		if (expr_type != TYPE_BOOL && expr_type != TYPE_ERR) {
			fprintf(p->syn_list, "SEMERR: Argument to \"if\" must be a boolean expression\n");
			ret = TYPE_ERR;
		}
		match(p, TOKEN_THEN, &ret);
		err_propogate(statement(p), &ret);
		err_propogate(statement_prime(p), &ret);
		break;
	}
	case TOKEN_WHILE: {
		int expr_type;
		match(p, TOKEN_WHILE, &ret);
		expr_type = expression(p);
		err_propogate(expr_type, &ret);
		if (expr_type != TYPE_BOOL && expr_type != TYPE_ERR) {
			fprintf(p->syn_list, "SEMERR: Argument to \"while\" must be a boolean expression\n");
			ret = TYPE_ERR;
		}
		match(p, TOKEN_DO, &ret);
		err_propogate(statement(p), &ret);
		break;
	}
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
