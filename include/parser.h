#ifndef PARSER_H
#define PARSER_H

#include "buffer.h"
#include "token.h"
//#include "table.h"

#define NO_ATTR -1

Token malar_next_token(Buffer*);

Token lookahead;
extern Buffer* sc_buf;
int synerrno;
extern int line;
extern char * kw_table[];
extern Buffer * str_LTBL;

void parser(Buffer*);
void gen_incode(char*);
void program();
void match(int, int);
void syn_eh(int);
void syn_printe();
void opt_statements();
void statements();
void statements_p();
void statement();
void assignment_statement();
void assignment_expression();
void iteration_statement();
void selection_statement();
void read_statement();
void write_statement();
void arithmetic_expression();
void unary_arithmetic_expression();
void primary_arithmetic_expression();
void string_expression();
void string_expression_p();
void primary_string_expression();
void pre_condition();
void conditional_expression();
void logical_OR_expression();
void logical_OR_expression_p();
void logical_AND_expression();
void logical_AND_expression_p();
void primary_a_relational_expression();
void primary_a_relational_expression_p();
void primary_s_relational_expression();
void primary_s_relational_expression_p();
void primary_string_expression();
void relational_expression();
void variable_list();
void variable_list_p();

#endif
