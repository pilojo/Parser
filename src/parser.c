#include "buffer.h"
#include "table.h"
#include "token.h"
#include "parser.h"



void parser(Buffer * in_buf) {
	sc_buf = in_buf;
	lookahead = malar_next_token(sc_buf);
	program(); match(SEOF_T, NO_ATTR);
	gen_incode("PLATY: Source file parsed");
}

void match(int pr_token_code, int pr_token_attribute)
{

}

void syn_eh(int sync_token_code)
{

}