/* -*- C -*-
 *
 * idres.c
 *
 * Author: Benjamin T James
 */

#include "idres.h"
#include "defs.h"
#include "util.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int idres_print(FILE* f, struct idres **list)
{
	struct idres *node = *list;
	while (node != NULL) {
		fprintf(f, "%p\t%s\n", node->token.val.ptr, node->lexeme);
		node = node->next;
	}
	return 0;
}

int idres_insert(struct idres **list, char* lexeme, struct token token)
{
	int ret = 0;
	struct idres *root = malloc(sizeof(*root));
	root->lexeme = lexeme;
	root->type = 0;
	root->token = token;
	root->next = *list;
	*list = root;
	return ret;
}
int idres_add_rw(struct idres **list, char* c_lexeme, int type, int attr)
{
	char *lexeme;
	struct token tok;
	if (sdup(c_lexeme, &lexeme) < 0) {
		return -1;
	}
	token_add(&tok, type, attr);
	return idres_insert(list, lexeme, tok);
}

int idres_add_id(struct idres **list, char* c_lexeme)
{
	char *lexeme;
	struct token tok;
	if (sdup(c_lexeme, &lexeme) < 0) {
		return -1;
	}
	token_id(&tok, lexeme);
	return idres_insert(list, lexeme, tok);
}

int idres_add_id_attr(struct idres **list, char* c_lexeme, char* attr)
{
	char *lexeme;
	struct token tok;
	if (sdup(c_lexeme, &lexeme) < 0) {
		return -1;
	}
	token_id(&tok, lexeme);
	tok.val.ptr = attr;
	return idres_insert(list, lexeme, tok);
}

int idres_lookup(struct idres **list, void* ptr, struct idres **ret)
{
	struct idres *node = *list;
	while (node != NULL) {
		if (ptr == node->token.val.ptr) {
			*ret = node;
			return 0;
		}
		node = node->next;
	}
	return -1;
}
int idres_find(struct idres *node, char *lexeme, struct idres **ret)
{
	while (node != NULL) {
		if (!strcmp(lexeme, node->lexeme)) {
			*ret = node;
			return 0;
		}
		node = node->next;
	}
	return -1;
}

int idres_search(struct idres **list, char* lexeme, struct idres **ret)
{
	return idres_find(*list, lexeme, ret);
}

int idres_clean(struct idres **list)
{
	while (*list != NULL) {
		struct idres *prev = *list;
		*list = prev->next;
		free(prev->lexeme);
		free(prev);
	}
	return 0;
}

int idres_read(const char *filename, struct idres **list)
{
	FILE* f = fopen(filename, "r");
	void* addr = NULL;
	long count;
	char *lexeme = malloc(ID_STRLEN + 1);
	/* strlen(lexeme) guaranteed to be ID_STRLEN */
	for (count = 0; fscanf(f, "0x%p\t%s\n", &addr, lexeme) == 2; count++) {
		idres_add_id_attr(list, lexeme, addr);
	}
	free(lexeme);
	fclose(f);
	/*return idres_balance(list, count);*/
	return 0;
}
