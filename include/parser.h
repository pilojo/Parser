#ifndef PARSER_H
#define PARSER_H

#include "buffer.h"
#include "token.h"

Token lookahead;
Buffer* sc_buf;
int synerrno;

void parser(Buffer*);
void gen_incode(char*);
void program();
void match(int, int);
void syn_eh(int);
void syn_printe();

#endif
