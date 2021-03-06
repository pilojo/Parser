/*
File name : parser.h
Compiler : MS Visual Studio 2015
Author : John Pilon, 040822687; Daniel Brenot, 040837035
Course : CST 8152 � Compilers, Lab Section : 12
Assignment : 3
Date : January 4th, 2018
Professor : Sv.Ranev
Purpose : Header to define functions and global variables used in the Parser
*/
#ifndef PARSER_H
#define PARSER_H

#include "buffer.h"
#include "token.h"

#define NO_ATTR -1

Token malar_next_token(Buffer*);

Token lookahead;
Buffer* sc_buf;
int synerrno;
extern int line;
extern char * kw_table[];
extern Buffer * str_LTBL;

void parser(Buffer*);
void gen_incode(char*);
void match(int, int);
void syn_eh(int);
void syn_printe();
void program();
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
void additive_arithmetic_expression();
void additive_arithmetic_expression_p();
void multiplicative_arithmetic_expression();
void multiplicative_arithmetic_expression_p();
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
void primary_s_relational_expression();
void primary_string_expression();
void relational_expression();
void relational_expression_p();
void relational_expression_s_p();
void variable_identifier();
void variable_list();
void variable_list_p();

#endif
