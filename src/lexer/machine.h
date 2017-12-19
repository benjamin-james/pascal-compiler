/* -*- C -*-
 *
 * machine.h
 *
 * Author: Benjamin T James
 */

#ifndef MACHINE_H
#define MACHINE_H

#include "defs.h"
#include "token.h"
#include "state.h"

struct machine {
	/* returns 1 on success, 0 on failure */
	int (*call)(struct machine *m, lex_state_t ls);

	char *f;
	char *b;
	struct token tok;
	struct machine *next;
};

/* interface for the lexer */
int machine_iter(lex_state_t ls, struct machine *state, char **out_str);

int machine_init(struct machine **list);
int machine_add(struct machine **list,
		int (*func)(struct machine *m, struct lex_state *ls));

int machine_clean(struct machine **list);

#endif
