/* -*- C -*-
 *
 * util.h
 *
 * Author: Benjamin T James
 */

#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>


int get_out_file(const char* in_file, const char* extension, char **out_file);
int get_file_without_ext(const char* f, char **to_write);
int get_str(char *f, char *b, char **ret);

int sdup(const char* s, char **ret);

#endif
