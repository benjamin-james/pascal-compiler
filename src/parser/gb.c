/* -*- C -*-
 *
 * gb.c
 *
 * Author: Benjamin T James
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gb.h"
#include "util.h"

#ifdef COLOR
 #define ANSI_COLOR_GREEN   "\x1b[32m"
 #define ANSI_COLOR_BLUE    "\x1b[34m"
 #define ANSI_COLOR_RESET   "\x1b[0m"
#else
 #define ANSI_COLOR_GREEN   ""
 #define ANSI_COLOR_BLUE    ""
 #define ANSI_COLOR_RESET   ""
#endif

int check_add_green(struct gb_state *gbs, char *lexeme,  int type)
{
	if (find_green(gbs->cur_eye, lexeme, NULL) == 1) {
		return -1;
	}
	struct gb *n_g = malloc(sizeof(*n_g));
	n_g->is_g = 1;
	sdup(lexeme, &n_g->n.g.lexeme);
	n_g->n.g.type = type;
	n_g->n.g.arglist = NULL;
	n_g->n.g.child = NULL;
	n_g->next = NULL;
	n_g->prev = gbs->cur_eye;
	eye_push(gbs, n_g);
	if (gbs->cur_eye != NULL) {
		gbs->cur_eye->next = n_g;
	}
	gbs->cur_eye = n_g;
#ifdef DEBUG
	printf("adding green node %s\n", lexeme);
	gb_print(*gbs);
#endif
	return 0;
}

int set_peek_args_type(struct gb_state *gbs, int type, const char *args)
{
	if (gbs->stack == NULL) {
		return -1;
	}
	gbs->stack->node->n.g.type = type;
	return sdup(args, &gbs->stack->node->n.g.arglist);
}
int gettype(struct gb_state *gbs, const char *lexeme, int *type)
{
	int ret = -1;
	struct gb *eye = gbs->cur_eye;
	while (eye != NULL) {
		if (eye->is_g &&
		    !strcmp(eye->n.g.lexeme,
			    lexeme)) {
			if (type) {
				*type = eye->n.g.type;
			}
			ret = 0;
			break;
		} else if (!eye->is_g && !strcmp(eye->n.b.lexeme,
				    lexeme)) {
			if (type) {
				*type = eye->n.b.type;
			}
			ret = 0;
			break;
		}
		eye = eye->prev;
	}
	return ret;
}

int check_add_blue(struct gb_state *gbs, char *lexeme, int type, int offset)
{
	if (find_blue(gbs->cur_eye, lexeme, NULL) == 1) {
		return -1;
	}
	struct gb *n_b = malloc(sizeof(*n_b));
	n_b->is_g = 0;
	sdup(lexeme, &n_b->n.b.lexeme);
	n_b->n.b.type = type;
	n_b->n.b.offset = offset;
	n_b->next = NULL;
	if (gbs->cur_eye == NULL) {
		fprintf(stderr, "adding blue when no green node exists\n");
		return -2;
	}
	gbs->cur_eye->next = n_b;
	n_b->prev = gbs->cur_eye;

	gbs->cur_eye = n_b;
#ifdef DEBUG
	printf("adding blue node %s\n", lexeme);
	gb_print(*gbs);
#endif
	return 0;

}

int end_green(struct gb_state *gbs)
{
	struct gb *gn = eye_pop(gbs);
	if (gn == NULL) {
		return -1;
	}
	gn->n.g.child = gn->next;
	gn->next = NULL;
	gbs->cur_eye = gn;
#ifdef DEBUG
	printf("ending green node %s\n", gn->n.g.lexeme);
	gb_print(*gbs);
#endif
	return 0;
}

int get_offset(struct gb_state *gbs, int *offset)
{
	if (gbs->stack != NULL) {
		if (offset) {
			*offset = gbs->stack->cur_offset;
		}
		return 0;
	} else {
		return -1;
	}
}

int set_offset(struct gb_state *gbs, int offset)
{
	if (gbs->stack != NULL) {
		gbs->stack->cur_offset = offset;
		return 0;
	} else {
		return -1;
	}
}

int gb_print(struct gb_state gbs)
{
	printf("eye stack:\n");
	struct eye *ptr = gbs.stack;
	while (ptr != NULL) {
		if (ptr->node->is_g) {
			printf("\t" ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n", ptr->node->n.g.lexeme);
		} else {
			printf("\t" ANSI_COLOR_BLUE  "%s" ANSI_COLOR_RESET "\n", ptr->node->n.b.lexeme);
		}
		ptr = ptr->next;
	}
	struct gb *e_ptr = gbs.cur_eye;
	printf("eye:\n");
	while (e_ptr != NULL) {
		if (e_ptr->is_g) {
			printf("\t" ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n", e_ptr->n.g.lexeme);
		} else {
			printf("\t" ANSI_COLOR_BLUE  "%s" ANSI_COLOR_RESET "\n", e_ptr->n.b.lexeme);
		}
		e_ptr = e_ptr->prev;
	}
	return 0;
}


int gb_state_init(struct gb_state *gbs)
{
	gbs->stack = NULL;
	gbs->cur_eye = NULL;
	return 0;
}

int eye_push(struct gb_state *gbs, struct gb* node)
{
	struct eye *n_eye = malloc(sizeof(*n_eye));
	if (n_eye == NULL) {
		return -1;
	}
	n_eye->next = gbs->stack;
	n_eye->node = node;
	n_eye->cur_offset = 0;
	gbs->stack = n_eye;
	return 0;
}

struct gb *eye_pop(struct gb_state *gbs)
{
	if (gbs->stack == NULL) {
		return NULL;
	}
	struct eye *next = gbs->stack->next;
	struct gb *ret = gbs->stack->node;
	free(gbs->stack);
	gbs->stack = next;
	return ret;
}

int gb_print_all(struct gb *ptr, FILE* f_out, int spaces)
{

	while (ptr != NULL) {
		if (ptr->is_g) {
			fprintf(f_out, "%*c" ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n", spaces, ' ', ptr->n.g.lexeme);
			gb_print_all(ptr->n.g.child, f_out, spaces+4);
		} else {
			fprintf(f_out, "%*c" ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET " %d\n", spaces, ' ', ptr->n.b.lexeme, ptr->n.b.offset);
		}
		ptr = ptr->next;
	}
	return 0;
}

int _free_green_node(struct gb** head)
{
	while (*head != NULL) {
		struct gb* ptr = *head;
		if (ptr->is_g) {
			free(ptr->n.g.lexeme);
			if (ptr->n.g.arglist != NULL) {
				free(ptr->n.g.arglist);
			}
			_free_green_node(&(ptr->n.g.child));
		} else {
			free(ptr->n.b.lexeme);
		}
		struct gb* next = ptr->next;
		free(ptr);

		*head = next;
	}
	return 0;
}
int gb_state_free(struct gb_state *gbs)
{
	while (gbs->stack != NULL) {
		eye_pop(gbs);
	}
        return _free_green_node(&gbs->cur_eye);

}
int find_gb(struct gb *cur_eye, char *lexeme, int g_p, struct gb **ret)
{
	while (cur_eye != NULL) {

		if (g_p == cur_eye->is_g) {
			char *eye_lexeme = g_p == 1 ? cur_eye->n.g.lexeme : cur_eye->n.b.lexeme;
			if (!strcmp(eye_lexeme, lexeme)) {
				if (ret != NULL) {
					*ret = cur_eye;
				}
				return 1;
			}
		}
		cur_eye = cur_eye->prev;
	}
	return 0;
}

int find_green(struct gb *cur_eye, char *lexeme, struct gb **ret)
{
	while (cur_eye != NULL) {
		if (1 == cur_eye->is_g) {
			char *eye_lexeme = cur_eye->n.g.lexeme;
			if (!strcmp(eye_lexeme, lexeme)) {
				if (ret != NULL) {
					*ret = cur_eye;
				}
				return 1;
			}
		}
		cur_eye = cur_eye->prev;
	}
	return 0;
}

int find_blue(struct gb *cur_eye, char *lexeme, struct gb **ret)
{
	while (cur_eye != NULL) {
		if (!cur_eye->is_g) {
			char *eye_lexeme = cur_eye->n.b.lexeme;
			if (!strcmp(eye_lexeme, lexeme)) {
				if (ret != NULL) {
					*ret = cur_eye;
				}
				return 1;
			}
		} else { /* green */
			char *eye_lexeme = cur_eye->n.g.lexeme;
			if (!strcmp(eye_lexeme, lexeme)) {
				if (ret != NULL) {
					*ret = cur_eye;
				}
				return 1;
			} else {
				return 0;
			}
		}
		cur_eye = cur_eye->prev;
	}
	return 0;
}
