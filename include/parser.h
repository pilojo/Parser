/*
File name : parser.h
Compiler : MS Visual Studio 2015
Author : John Pilon, 040822687; Daniel Brenot, 040837035
Course : CST 8152 ? Compilers, Lab Section : 12
Assignment : 3
Date : January 4th, 2018
Professor : Sv.Ranev
Purpose : Header to define functions and global variables used in the Parser
*/
#ifndef PARSER_H
#define PARSER_H

#include "buffer.h"
#include "token.h"
#include "symbol_table.h"

#define NO_ATTR -1

typedef struct LinkedListDescriptor { Token* current; struct LinkedListDescriptor * next; }LinkedList;
Token malar_next_token(Buffer*);

Token lookahead;
Buffer* sc_buf;
Buffer* out_buf;
SymbolTable *sc_st;
int synerrno;
extern int line;
extern char * kw_table[];
extern Buffer * str_LTBL;
LinkedList* tokenList;/*The linked list of variables currently being stored*/
char * currentline;/*The define statements in the list*/
char * currentStatements;/*The statements defined in the current scope*/
short statementsSize;/*The number of statements*/
short scopes = 1;


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
int arithmetic_expression();
int additive_arithmetic_expression();
int additive_arithmetic_expression_p();
int multiplicative_arithmetic_expression();
int multiplicative_arithmetic_expression_p();
int unary_arithmetic_expression();
int primary_arithmetic_expression();
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

/*Cross Compiler Functions*/
void appendDefine(int, char*);
void appendToken();
void clearList(LinkedList*);
#endif

