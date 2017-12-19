/* -*- C -*-
 *
 * types.h
 *
 * Author: Benjamin T James
 */

#ifndef TYPES_H
#define TYPES_H

#define TYPE_VOID        0
#define TYPE_ERR         1
#define TYPE_INT         2
#define TYPE_REAL        3
#define TYPE_BOOL        4

#define TYPE_AINT        5
#define TYPE_AREAL       6

#define TYPE_FINT        7
#define TYPE_FREAL       8

#define TYPE_PGMPARAM    9
#define TYPE_PGMNAME     10
#define TYPE_PLACEHOLDER 11

int ARRAY_TO_SCALAR(int atype);
int SCALAR_TO_ARRAY(int type);

int FUNC_TO_SCALAR(int ftype);
int SCALAR_TO_FUNC(int type);

int type_width(int type);
int err_propogate(int err, int *ret);
const char* strtype(int type);
char* param2str(const char* params);
#endif
