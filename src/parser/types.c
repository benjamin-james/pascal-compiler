/* -*- C -*-
 *
 * types.c
 *
 * Author: Benjamin T James
 */

#include "types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int FUNC_TO_SCALAR(int ftype)
{
	if (ftype == TYPE_FINT) {
		return TYPE_INT;
	} else if (ftype == TYPE_FREAL) {
		return TYPE_REAL;
	} else {
		return TYPE_ERR;
	}

}
int ARRAY_TO_SCALAR(int atype)
{
	if (atype == TYPE_AINT) {
		return TYPE_INT;
	} else if (atype == TYPE_AREAL) {
		return TYPE_REAL;
	} else {
		return TYPE_ERR;
	}
}
int SCALAR_TO_FUNC(int type)
{
	if (type == TYPE_INT) {
		return TYPE_FINT;
	} else if (type == TYPE_REAL) {
		return TYPE_FREAL;
	} else {
		return TYPE_ERR;
	}
}
int SCALAR_TO_ARRAY(int type)
{
	if (type == TYPE_INT) {
		return TYPE_AINT;
	} else if (type == TYPE_REAL) {
		return TYPE_AREAL;
	} else {
		return TYPE_ERR;
	}
}

int type_width(int type)
{
	if (type == TYPE_INT) {
		return 4;
	} else if (type == TYPE_REAL) {
		return 8;
	} else if (type == TYPE_PGMPARAM) {
		return 0;
	} else {
		return -1;
	}
}

int err_propogate(int err, int *ret)
{
	if (err == TYPE_ERR) {
		*ret = err;
	}
	return *ret;
}

const char* strtype(int type)
{
	switch (type) {
	case TYPE_INT:
		return "integer";
	case TYPE_REAL:
		return "real";
	case TYPE_AINT:
		return "array of integer";
	case TYPE_AREAL:
		return "array of real";
	case TYPE_BOOL:
		return "boolean";
	case TYPE_ERR:
		return "error";
	case TYPE_FINT:
		return "integer function";
	case TYPE_FREAL:
		return "real function";
	case TYPE_PGMNAME:
		return "program name";
	case TYPE_PGMPARAM:
		return "program parameter";
	case TYPE_PLACEHOLDER:
		return "PLACEHOLDER";
	case TYPE_VOID:
		return "VOID";
	default:
		return "undefined";
	}
}

char* param2str(const char* params)
{
	int i, len = strlen(params);
	char *oldbuf = calloc(1,1);
	for (i = 0; i < len; i++) {
		const char* param = strtype(params[i]);
		char *buf = malloc(strlen(param) + 2 + strlen(oldbuf) + 1);
		if (i > 0) {
			sprintf(buf, "%s, %s", oldbuf, param);
		} else {
			sprintf(buf, "%s", param);
		}
		free(oldbuf);
		oldbuf = buf;
	}
	return oldbuf;
}
