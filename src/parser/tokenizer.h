#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "token.h"
#include "defs.h"
#include "parser.h"

int next_token(struct parser *p);

int parser_listing(struct parser *p, unsigned lineno);
#endif
