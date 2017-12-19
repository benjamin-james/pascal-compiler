/* -*- C -*-
 *
 * fsm.c
 *
 * Author: Benjamin T James
 */

#include <ctype.h>
#include <string.h>
#include "fsm.h"
#include "defs.h"
#include "util.h"

int digit_plus(struct machine *m)
{
	int len = 1;
	if (!isdigit(*m->f)) {
		return 0;
	}
	m->f++;
	while (isdigit(*m->f)) {
		m->f++;
		len++;
	}
	return len;
}

int fsm_relop(struct machine *m, struct lex_state *ls)
{
	if (*m->f == '>') {
		m->f++;
		if (*m->f == '=') {
			token_add(&m->tok, TOKEN_RELOP, TOKEN_GEQ);
		} else {
			m->f--;
			token_add(&m->tok, TOKEN_RELOP, TOKEN_GT);
		}
	} else if (*m->f == '=') {
		token_add(&m->tok, TOKEN_RELOP, TOKEN_EQ);
	} else if (*m->f == '<') {
		m->f++;
		if (*m->f == '=') {
			token_add(&m->tok, TOKEN_RELOP, TOKEN_LEQ);
		} else if (*m->f == '>') {
			token_add(&m->tok, TOKEN_RELOP, TOKEN_NEQ);
		} else {
			m->f--;
			token_add(&m->tok, TOKEN_RELOP, TOKEN_LT);
		}
	} else {
		return 0;
	}
	return 1;
}
int fsm_integer(struct machine *m, struct lex_state *ls)
{
	char *lexeme;
	int result, len;
	if (!digit_plus(m)) {
		return 0;
	}
	m->f--;
	if (get_str(m->f, m->b, &lexeme) < 0) {
		return -1;
	}
	len = strlen(lexeme);
	if (len > 10) {
		token_add(&m->tok, LEXERR, LEXERR_INT_TOO_LONG);
	} else if ((lexeme[0] == '0' && len > 1)
		   || (lexeme[0] == '-' && lexeme[1] == '0' && len > 2)) {
		token_add(&m->tok, LEXERR, LEXERR_LEADING_ZERO);
	} else {
		result = strtol(lexeme, NULL, 10);
		token_add(&m->tok, TOKEN_NUM_INTEGER, result);
	}
	free(lexeme);
	return 1;
}

int fsm_real(struct machine *m, struct lex_state *ls)
{
	char *lexeme;
	double result;
	int len, mantis_len = 0, frac_len = 0;
	mantis_len = digit_plus(m);
	if (mantis_len == 0 || *m->f != '.') {
		return 0;
	}
	m->f++;
	frac_len = digit_plus(m);
	if (frac_len == 0) {
		return 0;
	}
	m->f--;
	if (get_str(m->f, m->b, &lexeme) < 0) {
		return -1;
	}

	len = strlen(lexeme);
	if (mantis_len > 5) {
		token_add(&m->tok, LEXERR, LEXERR_MANTIS_TOO_LONG);
	} else if (frac_len > 5) {
		token_add(&m->tok, LEXERR, LEXERR_FRAC_TOO_LONG);
	} else if (lexeme[0] == '0' || (lexeme[0] == '-' && lexeme[1] == '0')) {
		token_add(&m->tok, LEXERR, LEXERR_LEADING_ZERO);
	} /* else if (lexeme[len-1] == '0') { */
	/* 	token_add(&m->tok, LEXERR, LEXERR_TRAILING_ZERO); */
	/* }  */else {
		result = strtod(lexeme, NULL);
		token_add(&m->tok, TOKEN_NUM_REAL, (int)result);
	}
	free(lexeme);
	return 1;
}
int fsm_long_real(struct machine *m, struct lex_state *ls)
{
	char *lexeme;
	double result;
	int len, tz = 0, mantis_len = 0, frac_len = 0, exp_len = 0;
	mantis_len = digit_plus(m);
	if (mantis_len == 0 || *m->f != '.') {
		return 0;
	}
	m->f++;
	frac_len = digit_plus(m);
	if (/* frac_len == 0 ||  */*m->f != 'E') {
		return 0;
	}
	if (get_str(m->f - 1, m->b, &lexeme) < 0) {
		return -1;
	}
	if (lexeme[strlen(lexeme) - 1] == '0') {
		tz = 1; /* set trailing zero flag to 1 */
	}

	free(lexeme);
	m->f++;
	if (*m->f == '-' || *m->f == '+') {
		m->f++;
	}
	exp_len = digit_plus(m);
	/* if (exp_len == 0) { */
	/* 	/\* err *\/ */
	/* 	return 0; */
	/* } */
	m->f--;
	if (get_str(m->f, m->b, &lexeme) < 0) {
		return -1;
	}
	len = strlen(lexeme);
	if (frac_len == 0) {
		token_add(&m->tok, LEXERR, LEXERR_NO_FRAC);
	} else if (exp_len == 0) {
		token_add(&m->tok, LEXERR, LEXERR_NO_EXP);
	} else if (mantis_len > 5) {
		token_add(&m->tok, LEXERR, LEXERR_MANTIS_TOO_LONG);
	} else if (frac_len > 5) {
		token_add(&m->tok, LEXERR, LEXERR_FRAC_TOO_LONG);
	} else if (exp_len > 2) {
		token_add(&m->tok, LEXERR, LEXERR_EXP_TOO_LONG);
	} else if (lexeme[0] == '0' || (lexeme[0] == '-' && lexeme[1] == '0')) {
		token_add(&m->tok, LEXERR, LEXERR_LEADING_ZERO);
	} else if (tz || lexeme[len-1] == '0') {
		token_add(&m->tok, LEXERR, LEXERR_TRAILING_ZERO);
	} else {
		result = strtod(lexeme, NULL);
		token_add(&m->tok, TOKEN_NUM_REAL, (int)result);
	}
	free(lexeme);
	return 1;
}

int fsm_addop(struct machine *m, struct lex_state *ls)
{
	if (*m->f == '+') {
		token_add(&m->tok, TOKEN_SIGN, TOKEN_PLUS);
		return 1;
	} else if (*m->f == '-') {
		token_add(&m->tok, TOKEN_SIGN, TOKEN_MINUS);
		return 1;
	} else if (*m->f == 'o') {
		m->f++;
		if (*m->f == 'r') {
			token_add(&m->tok, TOKEN_ADDOP, TOKEN_OR);
			return 1;
		}
	}
	return 0;
}
int fsm_mulop(struct machine *m, struct lex_state *ls)
{
	if (*m->f == '*') {
		token_add(&m->tok, TOKEN_MULOP, TOKEN_TIMES);
		return 1;
	} else if (*m->f == '/') {
		token_add(&m->tok, TOKEN_MULOP, TOKEN_RDIV);
		return 1;
	} else if (*m->f == 'd') {
		m->f++;
		if (*m->f == 'i' && m->f++ && *m->f == 'v') {
			token_add(&m->tok, TOKEN_MULOP, TOKEN_IDIV);
			return 1;
		}
	} else if (*m->f == 'm') {
		m->f++;
		if (*m->f == 'o' && m->f++ && *m->f == 'd') {
			token_add(&m->tok, TOKEN_MULOP, TOKEN_MOD);
			return 1;
		}
	} else if (*m->f == 'a') {
		m->f++;
		if (*m->f == 'n' && m->f++ && *m->f == 'd') {
			token_add(&m->tok, TOKEN_MULOP, TOKEN_AND);
			return 1;
		}
	}
	return 0;
}

int fsm_catchall(struct machine *m, struct lex_state *ls)
{
	switch (*m->f) {
	case '[':
		token_add(&m->tok, TOKEN_LBRACKET, *m->f);
		return 1;
	case ']':
		token_add(&m->tok, TOKEN_RBRACKET, *m->f);
		return 1;
	case '(':
		token_add(&m->tok, TOKEN_LPAREN, *m->f);
		return 1;
	case ')':
		token_add(&m->tok, TOKEN_RPAREN, *m->f);
		return 1;
	case ',':
		token_add(&m->tok, TOKEN_COMMA, *m->f);
		return 1;
	case ';':
		token_add(&m->tok, TOKEN_SEMICOLON, *m->f);
		return 1;
	default:
		break;
	}
	if (*m->f == '.') {
		m->f++;
		if (*m->f == '.') {
			token_add(&m->tok, TOKEN_ELLIPSIS, TOKEN_ELLIPSIS);
		} else {
			m->f--;
			token_add(&m->tok, TOKEN_PERIOD, '.');
		}
		return 1;
	} else if (*m->f == ':') {
		m->f++;
		if (*m->f == '=') {
			token_add(&m->tok, TOKEN_ASSIGN, 0);
		} else {
			m->f--;
			token_add(&m->tok, TOKEN_COLON, ':');
		}
		return 1;
	}
	return 0;
}

int fsm_idres(struct machine *m, struct lex_state *ls)
{
	if (isalpha(*m->f)) {
		int len;
		char *lexeme;
		struct idres *result;
		m->f++;
		while (isalnum(*m->f)) {
			m->f++;
		}
		m->f--;


		if (get_str(m->f, m->b, &lexeme) < 0) {
			return -1;
		}
		len = strlen(lexeme);
		if (len > ID_STRLEN) {
			m->tok.type = LEXERR;
			m->tok.is_id = 0;
			m->tok.val.attr = LEXERR_ID_TOO_LONG;
		} else if (idres_search(&ls->rwords, lexeme, &result) == 0) {
			m->tok = result->token;
		} else if (idres_search(&ls->ids, lexeme, &result) == 0) {
			m->tok = result->token;
		} else {
			idres_add_id(&ls->ids, lexeme);
			m->tok = ls->ids->token;
		}
		free(lexeme);
		return 1;
	}
	return 0;
}
int fsm_unrecognized_symbol(struct machine *m, struct lex_state *ls)
{
	m->tok.type = LEXERR;
	m->tok.val.attr = LEXERR_UNREC_SYM;
	return 1;
}
int fsm_newline(struct machine *m, struct lex_state *ls)
{
	if (*m->f == '\r') {
		m->f++;
		if (*m->f == '\n') {
			m->tok.type = TOKEN_NEWLINE;
			return 1;
		}
	} else if (*m->f == '\n') {
		m->f++;
		m->tok.type = TOKEN_NEWLINE;
		return 1;
	}
	return 0;
}
int fsm_whitespace(struct machine *m, struct lex_state *ls)
{
	if (*m->f == ' ' || *m->f == '\t') {
		m->f++;
		while (*m->f == ' ' || *m->f == '\t') {
			m->f++;
		}
		m->f--;
		m->tok.type = TOKEN_WHITESPACE;
		return 1;
	}
	return 0;

}
