#ifndef PARSER_H
#define PARSER_H

#define NO_ATTR -1

Buffer* sc_buf;
int lookahead;
int synerrno;

void parser(Buffer * in_buf);
void match(int pr_token_code, int pr_token_attribute);
void syn_eh(int sync_token_code);
void syn_printe();
void gen_incode();


/*Parser code specified by parse tree*/
void program();

#endif
