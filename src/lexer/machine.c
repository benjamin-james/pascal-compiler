/* -*- C -*-
 *
 * machine.c
 *
 * Author: Benjamin T James
 */

#include "machine.h"
#include "fsm.h"
#include "util.h"

int machine_iter(struct lex_state *ls, struct machine *state, char **out_str)
{
	int ret;
	struct machine *m = ls->machines;
	for (; m != NULL; m = m->next) {
		m->b = state->f;
		m->f = m->b;
		m->tok.is_id = 0;
	        ret = m->call(m, ls);
		if (ret == 1) {
			state->tok = m->tok;
			state->f = m->f + 1;
			state->b = state->f;

			return get_str(m->f, m->b, out_str);
		} else if (ret == -1) {
			return -1;
		}
	}
	return -1;
}
int machine_init(struct machine **list)
{
	machine_add(list, fsm_unrecognized_symbol);
	machine_add(list, fsm_newline);

	machine_add(list, fsm_catchall);
	machine_add(list, fsm_relop);

	machine_add(list, fsm_integer);
	machine_add(list, fsm_real);
	machine_add(list, fsm_long_real);

	machine_add(list, fsm_idres);
	machine_add(list, fsm_addop);
	machine_add(list, fsm_mulop);

	machine_add(list, fsm_whitespace);
	return 0;
}
int machine_add(struct machine **list,
		int (*func)(struct machine *m, struct lex_state *ls))
{
	struct machine *m = malloc(sizeof(*m));
	if (m == NULL) {
		fprintf(stderr, "Unable to allocate resources\n");
		return -1;
	}
	m->call = func;
	m->next = *list;
	*list = m;
	return 0;
}
int machine_clean(struct machine **list)
{
	struct machine *head = *list;
	struct machine *tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		free(tmp);
	}
	return 0;
}
