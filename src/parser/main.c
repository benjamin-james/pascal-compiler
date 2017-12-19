#include <stdio.h>
#include "parser.h"
#include "tokenizer.h"

int main(int argc, char **argv)
{
	struct parser parser;
	if (argc != 6) {
		fprintf(stderr, "Usage: %s symbols tokens listing_in address_out listing_out\n", *argv);
		return -1;
	}
	parser_init(&parser, argv[1], argv[2], argv[3], argv[4], argv[5]);
/*	idres_print(stdout, &parser.symbols);*/
	parse(&parser);
	parser_cleanup(&parser);
	return 0;
}
