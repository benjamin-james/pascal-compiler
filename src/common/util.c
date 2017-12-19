/* -*- C -*-
 *
 * util.c
 *
 * Author: Benjamin T James
 */

#include "util.h"
#include <stdlib.h>
#include <string.h>

int get_file_without_ext(const char* f, char **to_write)
{
	char *loc, *buf;
        if (sdup(f, &buf) < 0) {
		return -1;
	}
	*to_write = buf;
	loc = strrchr(buf, '.');
	if (loc) {
		*loc = 0;
	} else {
		return -1;
	}
	return 0;
}

int get_out_file(const char* in_file, const char* extension, char **out_file)
{
	char *str, *buf;
	int i, ext_len, total_len;
	if (get_file_without_ext(in_file, &str) < 0) {
		fprintf(stderr, "File \"%s\" must have an extension\n", in_file);
		return -1;
	}
        i = strlen(str);
	ext_len = strlen(extension) + 1; /* for decimal */
        total_len = i + ext_len;
        buf = malloc(total_len + 1); /* for null terminator */
	if (buf == NULL) {
		fprintf(stderr, "Could not allocate resources\n");
		return -1;
	}
	buf[total_len] = 0;
	sprintf(buf, "%s.%s", str, extension);
	free(str);
	*out_file = buf;
	return 0;
}

int get_str(char *f, char *b, char **ret)
{
	int len = (f - b) + 1;
	char *buf = malloc(len + 1);
	if (buf == NULL) {
		fprintf(stderr, "Could not allocate resources\n");
		return -1;
	}
	memcpy(buf, b, len);
	buf[len] = 0;
	*ret = buf;
	return 0;
}

int sdup(const char* s, char **ret)
{
	int len = strlen(s);
	char *buf = malloc(len + 1);
	if (buf == NULL) {
		fprintf(stderr, "Could not allocate resources\n");
		return -1;
	}
	buf[len] = 0;
	memcpy(buf, s, len);
	*ret = buf;
	return 0;
}
