/* -*- C -*-
 *
 * fsm.h
 *
 * Author: Benjamin T James
 */

#ifndef FSM_H
#define FSM_H

#include "machine.h"
#include "state.h"

int fsm_unrecognized_symbol(struct machine *m, struct lex_state *ls);
int fsm_whitespace(struct machine *m, struct lex_state *ls);
int fsm_newline(struct machine *m, struct lex_state *ls);
int fsm_idres(struct machine *m, struct lex_state *ls);
int fsm_relop(struct machine *m, struct lex_state *ls);
int fsm_addop(struct machine *m, struct lex_state *ls);
int fsm_mulop(struct machine *m, struct lex_state *ls);
int fsm_catchall(struct machine *m, struct lex_state *ls);
int fsm_integer(struct machine *m, struct lex_state *ls);
int fsm_real(struct machine *m, struct lex_state *ls);
int fsm_long_real(struct machine *m, struct lex_state *ls);
#endif
