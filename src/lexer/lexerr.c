/* -*- C -*-
 *
 * lexerr.c
 *
 * Author: Benjamin T James
 */

#include "lexerr.h"

int print_error(FILE* listing, int err, const char *lexeme)
{
	fprintf(listing, "LEXERR:\t");
	switch (err) {
	case LEXERR_ID_TOO_LONG:
		fprintf(listing, "ID too long:");
		break;
	case LEXERR_UNREC_SYM:
		fprintf(listing, "Unrecognized symbol:");
		break;
	case LEXERR_INT_TOO_LONG:
		fprintf(listing, "Int too long:");
		break;
	case LEXERR_MANTIS_TOO_LONG:
		fprintf(listing, "Mantissa too long:");
		break;
	case LEXERR_FRAC_TOO_LONG:
		fprintf(listing, "Fraction too long:");
		break;
	case LEXERR_LEADING_ZERO:
		fprintf(listing, "Leading zero:");
		break;
	case LEXERR_TRAILING_ZERO:
		fprintf(listing, "Trailing zero:");
		break;
	case LEXERR_LINE_TOO_LONG:
		fprintf(listing, "Line too long:");
		break;
	case LEXERR_EXP_TOO_LONG:
		fprintf(listing, "Exponent too long:");
		break;
	case LEXERR_NO_EXP:
		fprintf(listing, "No exponent:");
		break;
	case LEXERR_NO_FRAC:
		fprintf(listing, "No fractional part:");
		break;
	default:
		fprintf(listing, "Unknown error %d:", err);
	}
	fprintf(listing, "\t\t%s\n", lexeme);
	return 0;
}
