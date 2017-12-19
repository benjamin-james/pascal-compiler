/* -*- C -*-
 *
 * gb.h
 *
 * Author: Benjamin T James
 */

#ifndef GB_H
#define GB_H

#include <stdio.h>

struct green {
	char *lexeme;
	int type;
	char *arglist;
	struct gb *child;
};
struct blue {
	char *lexeme;
	int type;
	int offset;
};

union gb_node {
	struct green g;
	struct blue b;
};

struct gb {
	unsigned is_g : 1;
	union gb_node n;
	struct gb *next;
	struct gb *prev;
};

struct eye {
	struct gb *node;
	int cur_offset;
	struct eye *next;
};

struct gb_state {
	struct eye *stack;
	struct gb *cur_eye;
};


int check_add_green(struct gb_state *gbs, char *lexeme,  int type);
int check_add_blue(struct gb_state *gbs, char *lexeme, int type, int offset);
int end_green(struct gb_state *gbs);
int gettype(struct gb_state *gbs, const char* lexeme, int *type);

int get_offset(struct gb_state *gbs, int *offset);
int set_offset(struct gb_state *gbs, int offset);

int gb_state_init(struct gb_state *gbs);
int gb_state_free(struct gb_state *gbs);
int set_peek_args_type(struct gb_state *gbs, int type, const char *args);
struct gb *eye_pop(struct gb_state *gbs);
int eye_push(struct gb_state *gbs, struct gb* node);


int find_green(struct gb *cur_eye, char *lexeme, struct gb **ret);
int find_blue(struct gb *cur_eye, char *lexeme, struct gb **ret);

int gb_print(struct gb_state gbs);
int gb_print_all(struct gb *ptr, FILE* out, int spaces);

#endif
