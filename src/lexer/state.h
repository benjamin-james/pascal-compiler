/* -*- C -*-
 *
 * state.h
 *
 * Author: Benjamin T James
 */

#ifndef STATE_H
#define STATE_H

#include <stdio.h>
#include "defs.h"
#include "io.h"
#include "idres.h"
#include "machine.h"

struct lex_state {
	/* inputs */
	FILE* source;
	FILE* res_word;

	/* outputs */
	FILE* sym;
	FILE* list;
	FILE* token;

	/* lexer state */
	struct line buf;
	struct idres *rwords;
	struct idres *ids;
        machine_t machines;
};

int state_init(const char *source, const char *res_word,
	       int line_len, struct lex_state *st);
int resword_init(struct lex_state *s);
int state_cleanup(struct lex_state *s);
#endif
