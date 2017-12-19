/* -*- C -*-
 *
 * io.c
 *
 * Author: Benjamin T James
 */

#include <stdlib.h>
#include <stddef.h>
#include "defs.h"
#include "io.h"
#include "util.h"

int read_line(struct line *buf, FILE *f)
{
	int c, ret = 0;
	unsigned offset = 0;
        buf->len = 0;
	while ((c = getc(f)) != EOF) {
		if (offset == buf->alloc) {
			buf->len += offset;
			offset = 0;
			buf->err = LEXERR_LINE_TOO_LONG;
		}
		buf->buf[offset++] = c;
		if (c == '\n') {
			buf->buf[offset] = '\0';
			break;
		}
	}
	buf->len += offset;
	if (c == EOF) {
		ret = -1;
	}
	return ret;
}

int open_file(const char* src_file, const char* ext,  FILE** out)
{
	char *out_name;
	FILE *f;
	if (get_out_file(src_file, ext, &out_name) < 0) {
		return -1;
	}
        f = fopen(out_name, "w");
	*out = f;
	free(out_name);
	if (f == NULL) {
		fprintf(stderr, "Could not open file \"%s\"\n", out_name);
		return -1;
	}

	return f == NULL ? -1 : 0;
}

int init_buf(struct line* l, size_t alloc)
{
	l->buf = malloc(alloc + 1);
	if (l->buf == NULL) {
		fprintf(stderr, "Could not allocate resources\n");
		return -1;
	}
	l->buf[alloc] = 0;
	l->alloc = alloc;
	l->err = 0;
	l->len = 0;
	return 0;
}

int free_buf(struct line *l)
{
	if (l->buf != NULL) {
		free(l->buf);
	}
	return 0;
}
