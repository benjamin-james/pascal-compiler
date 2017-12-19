/* -*- C -*-
 *
 * io.h
 *
 * Author: Benjamin T James
 */

#ifndef IO_H
#define IO_H
#include <stdio.h>
#include <stddef.h>

struct line {
	char *buf;
	int len;
	size_t alloc;
	int err;
};

int open_file(const char* src_file, const char* ext, FILE** out);
int init_buf(struct line* l, size_t alloc);
int free_buf(struct line* l);

int read_line(struct line* l, FILE *f);

#endif
