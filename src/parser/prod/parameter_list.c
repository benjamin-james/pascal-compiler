/* -*- C -*-
 *
 * parameter_list.c
 *
 * Author: Benjamin T James
 */

#include <string.h>
#include "parameter_list.h"
#include "type.h"
#include "parameter_list_prime.h"

#define INTBUF 10

int parameter_list(struct parser *p, char **args)
{
	int sync_set[] = {TOKEN_RPAREN};
	int expected[] = {TOKEN_ID};
	const int sync_size = sizeof(sync_set)/sizeof(*sync_set);
	const int expected_size = sizeof(expected)/sizeof(*expected);
	struct token t_id;
	int ret = TYPE_VOID;
	int width, var_type, offset;
	char *lexeme = NULL;
	switch (p->token.type) {
	case TOKEN_ID:
		t_id = p->token;
		match(p, TOKEN_ID, &ret);
		match(p, TOKEN_COLON, &ret);
		lexeme = parser_get_tok_lex(p, t_id);
		var_type = type(p, &width);
		err_propogate(var_type, &ret);
		get_offset(&p->gbs, &offset);
		int bret = check_add_blue(&p->gbs, lexeme, var_type, 0);
		if (lexeme != NULL) {
			if (bret == -1) {
				fprintf(p->syn_list,
					"SEMERR: Identifier \"%s\" already declared\n",
					lexeme);
				ret = TYPE_ERR;
			} else if (bret < 0) {
				ret = TYPE_ERR;
			}
		}
		/* set_offset(&p->gbs, offset + width); */
		err_propogate(parameter_list_prime(p, args), &ret);
		int len = INTBUF + 1;
		if (*args != NULL) {
			len += strlen(*args);
		}
		char *next_str = malloc(len);
		*next_str = (char)var_type;
		next_str[1] = 0;
		if (*args != NULL) {
			strcpy(next_str+1, *args);
			free(*args);
		}
		*args = next_str;
		break;
	default:
		ret = TYPE_ERR;
		expected_found(p, expected, expected_size);
		sync(p, sync_set, sync_size);
	}
	return ret;
}
