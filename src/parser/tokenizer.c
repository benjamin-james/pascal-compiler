#include "tokenizer.h"
#include <string.h>
#include <stdlib.h>

int next_token(struct parser *p)
{
	unsigned lineno;
	int attr;
	if (fscanf(p->f_token, "%u", &lineno) == EOF) {
		fprintf(stderr, "warning: EOF token may not be included in token file\n");
		return -1;
	}
	fscanf(p->f_token, "%s", p->buffer);
	fscanf(p->f_token, "%d", &p->token.type);
	if (p->token.type == TOKEN_ID) {
		struct idres *tok = NULL;
		if (idres_search(&p->symbols, p->buffer, &tok) == -1) {
			fprintf(stderr, "idres can't find symbol %s\n", p->buffer);
			return -1;
		}
		p->token = tok->token;
		fscanf(p->f_token, "%s\n", p->buffer);
	} else {
		fscanf(p->f_token, "%d\n", &attr);
		p->token.is_id = 0;
		p->token.val.attr = attr;
		if (!strcmp(token2str(p->token.type), "UNKNOWN")) {
			fprintf(p->syn_list, "SYNERR: Unknown token encountered: %d\n", attr);
		}
	}
	parser_listing(p, lineno);

	return p->token.type;
}

int g_temp = 0;
int parser_listing(struct parser *p, unsigned new_tok_lineno)
{
	int c;
        unsigned cur_tok_lineno = p->tok_line;
	if (cur_tok_lineno == new_tok_lineno) {
		return 0;
	} else if (g_temp == 0) {
		g_temp = 1;
		/* return 0; */
	} else {
		g_temp = 0;
	}
	p->tok_line = new_tok_lineno;
	while (p->list_line < p->tok_line) {
		int fret = fscanf(p->lex_list, "%s", p->buffer);
		if (fret == -1) { /* EOF reached */
			p->list_line = p->tok_line;
			break;
		}
		if (strcmp(p->buffer, "LEXERR:")) {
			char *ptr = NULL;
			p->list_line = strtoul(p->buffer, &ptr, 10);
			if (*ptr) {
				fprintf(p->syn_list, "Listing file invalid format, fret %d\n", fret);
				exit(EXIT_FAILURE);
			}
		}
		fprintf(p->syn_list, "%s", p->buffer);
		while ((c = fgetc(p->lex_list)) != '\n') {
			if (c == EOF) {
				p->list_line = p->tok_line;
				break;
			}
			fputc(c, p->syn_list);
		}
		fputc('\n', p->syn_list);
	}
	return 0;
}
