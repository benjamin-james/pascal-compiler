/* -*- C -*-
 *
 * idres.h
 *
 * Author: Benjamin T James
 */

#ifndef IDRES_H
#define IDRES_H

#include <stdlib.h>
#include "token.h"
#include "io.h"

struct idres {
	char *lexeme;
	int type;
	struct token token;
	struct idres *next;
};

int idres_add_rw(struct idres **list, char* lexeme, int token, int attr);
int idres_add_id(struct idres **list, char* lexeme);

int idres_search(struct idres **list, char* lexeme, struct idres **ret);
int idres_lookup(struct idres **list, void* ptr, struct idres **ret);
int idres_clean(struct idres **list);
int idres_print(FILE* f, struct idres **list);

int idres_read(const char *filename, struct idres **list);

int idres_add_id_attr(struct idres **list, char* lexeme, char* attr);
#endif
