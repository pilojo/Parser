/*
File name : parser.c
Compiler : MS Visual Studio 2015
Author : John Pilon, 040822687; Daniel Brenot, 040837035
Course : CST 8152 – Compilers, Lab Section : 12
Assignment : 3
Date : January 4th, 2018
Professor : Sv.Ranev
Purpose : Parses the tokens collected by the scanner according to the PLATYPUS grammar specifications
Function list :
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
*/

#include <stdlib.h>

#include "parser.h"

#define ELSE 0
#define FALSE 1
#define IF 2
#define PLATYPUS 3
#define READ 4
#define REPEAT 5
#define THEN 6
#define TRUE 7
#define WHILE 8
#define WRITE 9

/*
Purpose: Begins the parsing program
Author : Svillen Ranev
History / Versions : 1.0
Called functions : malar_next_token(Buffer*), program(), match(int, int)
Parameters : in_buf, buffer used for the scanner
*/
void parser(Buffer* in_buf){
	currentStatements = NULL;
	currentline = NULL;
	sc_buf = in_buf;
	sc_st = t_allocate(1000);/*Allocates symbol table*/
	lookahead = malar_next_token(sc_buf);
	program(); match(SEOF_T, NO_ATTR);
}

/*
Purpose: Generates a message after a production has been parsed
Author : Daniel Brenot
History / Versions : 1.0
Called functions : printf(char*, char*...)
Parameters : message, the message to be printed
*/
void gen_incode(char* message)
{
	int newLen = (strlen(message) + (currentline?strlen(currentline):0) + 1);
	char * new = (char *)malloc(sizeof(char)*newLen);
	
	
	
	sprintf(new, "%s%s", currentline?currentline:"", message);
	if (currentline)
	{
		free(currentline);
		currentline = NULL;
	}
	currentline = new;
	if (currentline[newLen - 2]=='\n')
	{
		if (strlen(message) == 2 && message[0] == '}')
		{
			newLen += currentStatements ? strlen(currentStatements) : 0;
			newLen += scopes - 1;
			new = (char*)malloc(sizeof(char)*newLen);
			sprintf(new, "%s%s", currentStatements ? currentStatements : "", currentline);
			while (scopes > 1)
			{
				sprintf(new, "%s%s", new, "}");
				scopes--;
			}
		}
		else
		{
			newLen += currentStatements ? strlen(currentStatements) : 0;
			new = (char*)malloc(sizeof(char)*newLen);
			sprintf(new, "%s%s", currentStatements ? currentStatements : "", currentline);
		}
		if (currentStatements) {
			free(currentStatements);
			currentStatements = NULL;
		}
		if (currentline)
		{
			free(currentline);
			currentline = NULL;
		}
		currentStatements = new;
		
	}
}

/*
Purpose: Checks to see if the token given by the scanner matches the expected token
Authors: John Pilon, Daniel Brenot
History / Versions : 1.5
Called functions : malar_next_token(Buffer*), syn_printe(), syn_eh(int)
Parameters : pr_token_code, token code to match; pr_token_attribute, token attribute to match
*/
void match(int pr_token_code, int pr_token_attribute){
	
	if(lookahead.code == pr_token_code){
		
		switch(pr_token_code){
			
			case KW_T:case LOG_OP_T:case ART_OP_T:case REL_OP_T:
				
				if(lookahead.attribute.get_int != pr_token_attribute) break;
				
			default:
				
				if(lookahead.code == SEOF_T) return;
				
				lookahead = malar_next_token(sc_buf);
				
				if(lookahead.code == ERR_T){
					syn_printe();
					lookahead = malar_next_token(sc_buf);
					synerrno++;
				}
				
				return;
		}
	}
	synerrno++;
	syn_printe();
	syn_eh(pr_token_code);
}

/*
Purpose: Handles errors
Author : Daniel Brenot
History / Versions : 1.2
Called functions : malar_next_token(Buffer*), exit();
Parameters : sync_token_code, the token code of a token that would be safe to continue parsing with
Algorithm: Continually search for tokens until a safe token is found
*/
void syn_eh(int sync_token_code){
	while(lookahead.code != SEOF_T){
		
		lookahead = malar_next_token(sc_buf);
		
		if(lookahead.code == sync_token_code){
			if(lookahead.code != SEOF_T) lookahead = malar_next_token(sc_buf);
			
			return;	
		}
	}
	
	if(sync_token_code != SEOF_T){
		exit(synerrno);	
	}
}

/*
Purpose: Prints the error
Author : Svillen Ranev
History / Versions : 1.0
Called functions : printf(char*, char*...), b_location(Buffer*, int)
*/
void syn_printe() {
	Token t = lookahead;

	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case  ERR_T: /* ERR_T     0   Error token */
		printf("%s\n", t.attribute.err_lex);
		break;
	case  SEOF_T: /*SEOF_T    1   Source end-of-file token */
		printf("NA\n");
		break;
	case  AVID_T: /* AVID_T    2   Arithmetic Variable identifier token */
	case  SVID_T:/* SVID_T    3  String Variable identifier token */
		printf("%s\n", t.attribute.vid_lex);
		break;
	case  FPL_T: /* FPL_T     4  Floating point literal token */
		printf("%5.1f\n", t.attribute.flt_value);
		break;
	case INL_T: /* INL_T      5   Integer literal token */
		printf("%d\n", t.attribute.get_int);
		break;
	case STR_T:/* STR_T     6   String literal token */
		printf("%s\n", b_location(str_LTBL, t.attribute.str_offset));
		break;

	case SCC_OP_T: /* 7   String concatenation operator token */
		printf("NA\n");
		break;

	case  ASS_OP_T:/* ASS_OP_T  8   Assignment operator token */
		printf("NA\n");
		break;
	case  ART_OP_T:/* ART_OP_T  9   Arithmetic operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  REL_OP_T: /*REL_OP_T  10   Relational operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  LOG_OP_T:/*LOG_OP_T 11  Logical operator token */
		printf("%d\n", t.attribute.get_int);
		break;

	case  LPR_T: /*LPR_T    12  Left parenthesis token */
		printf("NA\n");
		break;
	case  RPR_T: /*RPR_T    13  Right parenthesis token */
		printf("NA\n");
		break;
	case LBR_T: /*    14   Left brace token */
		printf("NA\n");
		break;
	case RBR_T: /*    15  Right brace token */
		printf("NA\n");
		break;

	case KW_T: /*     16   Keyword token */
		printf("%s\n", kw_table[t.attribute.get_int]);
		break;

	case COM_T: /* 17   Comma token */
		printf("NA\n");
		break;
	case EOS_T: /*    18  End of statement *(semi - colon) */
		printf("NA\n");
		break;
	default:
		printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
	}/*end switch*/
}

/********************************************************************************************

	PRODUCTION FUNCTIONS

*********************************************************************************************/

/*
   <program>  ->
    PLATYPUS {<opt_statements>}

   FIRST SET = { PLATYPUS, { }

   Author: John Pilon
*/
void program(){
	int i;
	printf("void main(int argc, char**argv){\n");
	match(KW_T, PLATYPUS); match(LBR_T, NO_ATTR);
	opt_statements();
	printf("%s",currentStatements?currentStatements:NULL);
	/*Appends the closing braces*/
	for (i = 0; i < scopes; i++) { printf("}\n"); }
	match(RBR_T, NO_ATTR);
}

/*
   <opt_statements> ->
    <statements> | E

   FIRST SET = { AVID, SVID, IF, READ, WRITE, WHILE, E }

   Author: John Pilon
*/
void opt_statements(){
	
	switch(lookahead.code){
		case KW_T:
			
			switch(lookahead.attribute.get_int){
				
				case PLATYPUS:case ELSE:case THEN:case REPEAT:
					return;
					
				default:
					break;
			}
			
		case AVID_T:case SVID_T:
			statements();
			break;
	}
}
/*
<statements> -> <statement><statements’>

FIRST SET = { AVID, SVID, IF, READ, WRITE, WHILE, E }

Author: John Pilon
*/
void statements(){
	statement();
	statements_p();
}

/*
	
<statements’> -> <statement><statements’> | E

FIRST SET = { AVID, SVID, IF, READ, WRITE, WHILE, E }

Author: John Pilon
*/
void statements_p(){
	switch(lookahead.code){
		case KW_T:
			switch(lookahead.attribute.get_int){
				case PLATYPUS:case ELSE:case THEN:case REPEAT:
					return;
			}
		case AVID_T: case SVID_T:
			statement();
			statements_p();
			break;
	}
}

/*
<statement> ->
<assignment statement>
| <selection statement>
| <iteration statement>
| <input statement>
| <output statement>

FIRST SET = { AVID, SVID, IF, READ, WRITE, WHILE }

Author: John Pilon
*/
void statement(){
	switch(lookahead.code){
		case AVID_T: case SVID_T:
			assignment_statement();
			break;
		case KW_T:
			switch(lookahead.attribute.get_int){
				case IF:
					selection_statement();
					break;
				case WHILE:
					iteration_statement();
					break;
				case READ:
					read_statement();
					break;
				case WRITE:
					write_statement();
					break;
				default:
					syn_printe();
			}
			break;
		default:
			syn_printe();
	}	
}

/*
<assignment statement> ->
<assignment expression>;

FIRST SET = { AVID, SVID }

Author: John Pilon
*/
void assignment_statement() {
	assignment_expression();
	gen_incode(";\n");
	match(EOS_T, NO_ATTR);
}
/*
< assignment expression> ->
AVID = <arithmetic expression>
| SVID = <string expression>

FIRST SET = { AVID, SVID }

Author: John Pilon
*/
void assignment_expression() {
	unsigned char ret;
	Token token;
	switch (lookahead.code) {
	case AVID_T:
		ret = t_locate(*sc_st, lookahead.attribute.vid_lex, &token);
		if (ret == NOTFOUND)
		{
			lookahead.type = INTEGER;
			table_adds(sc_st, lookahead);
			token = lookahead;
		}


		gen_incode(lookahead.attribute.vid_lex);
		match(AVID_T, NO_ATTR);
		gen_incode("=");
		match(ASS_OP_T, NO_ATTR);
		token.type = arithmetic_expression();
		if (ret == INTEGER &&  token.type== FLOAT)
		{
			/*Shadow scope and redefine as float*/
			printf("%s{",currentStatements?currentStatements:"");
			if (currentStatements)
			{
				free(currentStatements);
				currentStatements = NULL;
			}
			scopes++;
		}
		else if (ret == NOTFOUND)
		{
			char * new = (char*)malloc(sizeof(char)*17);
			int newLen = (16 + (currentStatements?strlen(currentStatements):0) + 1);
			/*Add a define to the beginning of current scope*/
			switch (token.type)
			{
			case INTEGER:sprintf(new, "int %s;\n", token.attribute.vid_lex);
				break;
			case FLOAT:sprintf(new, "float %s;\n",token.attribute.vid_lex);
				break;
			}
			
			
			char * appended = (char *)malloc(sizeof(char)*newLen);
			sprintf(appended, "%s%s", new, currentStatements ? currentStatements : "");
			if (currentStatements)free(currentStatements);
			currentStatements = appended;
		}
		break;
	case SVID_T:
		ret = t_locate(*sc_st, lookahead.attribute.vid_lex, &token);
		if (ret == NOTFOUND)
		{
			lookahead.type = STRING;
			table_adds(sc_st, lookahead);
		}
		gen_incode("char * ");
		gen_incode(lookahead.attribute.vid_lex);
		match(SVID_T, NO_ATTR);
		gen_incode("=");
		match(ASS_OP_T, NO_ATTR);
		string_expression();
		break;
	default:
		syn_printe();
		return;
	}
}
/*
<iteration statement> ->
WHILE <pre-condition> (<conditional expression>)
REPEAT { <statements>};

FIRST SET = { WHILE, '(' }

Author: John Pilon
*/
void iteration_statement() {
	gen_incode("while");
	match(KW_T, WHILE);
	pre_condition();
	match(LPR_T, NO_ATTR);
	conditional_expression();
	gen_incode("))");
	match(RPR_T, NO_ATTR);
	match(KW_T, REPEAT);
	gen_incode("{\n");
	match(LBR_T, NO_ATTR);
	statements();
	gen_incode("}");
	match(RBR_T, NO_ATTR);
	gen_incode("\n");
	match(EOS_T, NO_ATTR);
}
/*
<selection statement> ->
IF TRUE (<conditional expression>) THEN { <opt_statements> }
ELSE { <opt_statements> } ;

FIRST SET = { IF, TRUE }

Author: John Pilon
*/
void selection_statement() {
	gen_incode("if");
	match(KW_T, IF);
	match(KW_T, TRUE);
	gen_incode("(");
	match(LPR_T, NO_ATTR);
	conditional_expression();
	gen_incode(")");
	match(RPR_T, NO_ATTR);
	match(KW_T, THEN);
	gen_incode("{\n");
	match(LBR_T, NO_ATTR);
	opt_statements();
	gen_incode("}\n");
	match(RBR_T, NO_ATTR);
	gen_incode("else");
	match(KW_T, ELSE);
	gen_incode("{\n");
	match(LBR_T, NO_ATTR);
	opt_statements();
	gen_incode("}");
	match(RBR_T, NO_ATTR);
	gen_incode("\n");
	match(EOS_T, NO_ATTR);
}

/*<input statement> ->
READ (<variable list>);

FIRST SET = { READ, '(' }

Author: John Pilon
*/
void read_statement() {
	LinkedList* iterator;
	match(KW_T, READ);
	match(LPR_T, NO_ATTR);
	variable_list();
	iterator = tokenList;
	/*Create all print statements here*/
	while (iterator)
	{
		switch (iterator->current->type)
		{
		case INTEGER:gen_incode("scanf(\"%d\\n\",&"); break;
		case FLOAT:gen_incode("scanf(\"%f\\n\",&");
		}
		gen_incode(iterator->current->attribute.vid_lex);
		gen_incode(");\n");
		iterator = iterator->next;
	}

	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
}

/*
<output statement> ->
WRITE (<output list>);

FIRST SET = { WRITE, '(' }

Author: John Pilon
*/
void write_statement() {
	LinkedList* iterator;
	int strSize=0, i=0;
	char * temp;
	match(KW_T, WRITE); match(LPR_T, NO_ATTR);
	switch (lookahead.code) {
	case AVID_T:case SVID_T:
		variable_list();
		iterator = tokenList;
		while (iterator)
		{
			if (iterator->current) {
				switch (iterator->current->type)
				{
				case INTEGER:gen_incode("printf(\"%d\\n\","); break;
				case FLOAT:gen_incode("printf(\"%f\\n\",");
				}

				gen_incode(iterator->current->attribute.vid_lex);
				gen_incode(");\n");
				iterator = iterator->next;
			}
		}
		break;
	case STR_T:
		
		gen_incode("printf(\"");
		b_mark(str_LTBL, lookahead.attribute.str_offset);
		b_reset(str_LTBL);
		while (b_getc(str_LTBL) != '\0') { strSize++; }
		temp = (char*)malloc(sizeof(char)*(strSize+1));
		b_reset(str_LTBL);
		while (strSize >= 0) { temp[i] = b_getc(str_LTBL); strSize--; i++; }
		gen_incode(temp);
		gen_incode("\");\n");
		match(STR_T, NO_ATTR);
		break;
	case RPR_T:gen_incode("printf(\"\\n\");\n");
		break;
	}
	match(RPR_T, NO_ATTR); match(EOS_T, NO_ATTR);
}

/*
<arithmetic expression> - >
<unary arithmetic expression>
| <additive arithmetic expression>

FIRST SET = { -, +, AVID, FPL, INL, '(' }

Author: John Pilon
*/
int arithmetic_expression(){
	int ret = NOTFOUND;
	switch(lookahead.code){
	case ART_OP_T:
		switch (lookahead.attribute.arr_op) {
		case MULT:case DIV:
			syn_printe();
			break;
		}
		ret = unary_arithmetic_expression();
		break;
	case AVID_T: case FPL_T: case INL_T: case LPR_T:
		ret = additive_arithmetic_expression();
		break;
	case EOS_T:
		return;
	default:
		syn_printe();
	}
	return ret;
}

/*
<additive arithmetic expression> ->
<multiplicative arithmetic expression> <additive arithmetic expression'>

FIRST SET = { +, *, AVID, FPL, INL, ( }

Author: Daniel Brenot
*/
int additive_arithmetic_expression(){
	int ret = multiplicative_arithmetic_expression();
	ret = additive_arithmetic_expression_p()==FLOAT?FLOAT:ret;
	return ret;
}

/* 
<additive arithmetic expression'> ->
+ <multiplicative arithmetic expression> <additive arithmetic expression'>
| - <multiplicative arithmetic expression> <additive arithmetic expression'>


FIRST SET = { +, - }

Author: Daniel Brenot
*/
int additive_arithmetic_expression_p()
{
	int ret = NOTFOUND;
	if (lookahead.code == ART_OP_T &&lookahead.attribute.arr_op != MULT && lookahead.attribute.arr_op != DIV)
	{
		switch (lookahead.attribute.arr_op)
		{
		case MULT:
		case DIV:return;
		case PLUS:gen_incode("+");
			break;
		case MINUS:gen_incode("-");
			break;
		}
		match(lookahead.code, lookahead.attribute.arr_op);
		ret = multiplicative_arithmetic_expression();
		ret = additive_arithmetic_expression_p() == FLOAT ? FLOAT:ret;
	}
	return ret;
}

/*
<multiplicative arithmetic expression> ->
<primary arithmetic expression> <multiplicative arithmetic expression'>

FIRST SET = { AVID, FPL, INL, '(' } 

Author: Daniel Brenot
*/
int multiplicative_arithmetic_expression(){
	int ret = primary_arithmetic_expression();
	return multiplicative_arithmetic_expression_p()==FLOAT?FLOAT:ret;
}

/*
<multiplicative arithmetic expression'> ->
	* <primary arithmetic expression> <multiplicative arithmetic expression'>
	| / <primary arithmetic expression> <multiplicative arithmetic expression'>

FIRST_SET = { *, / } 

Author: John Pilon
*/
int multiplicative_arithmetic_expression_p() {
	int ret = NOTFOUND;
	if (lookahead.code == ART_OP_T)
	{
		switch (lookahead.attribute.arr_op)
		{
		case MULT:
			gen_incode("*");
			match(lookahead.code, lookahead.attribute.arr_op);
			ret = primary_arithmetic_expression();
			if (multiplicative_arithmetic_expression_p() == FLOAT) ret = FLOAT;
			break;
		case DIV:
			gen_incode("/");
			match(lookahead.code, lookahead.attribute.arr_op);
			primary_arithmetic_expression();
			multiplicative_arithmetic_expression_p();
			ret = FLOAT;
			break;
		}

	}
	return ret;
}

/*
<unary arithmetic expression> ->
-  <primary arithmetic expression>
| + <primary arithmetic expression>


FIRST SET = { -, + }

Author: John Pilon
*/
int unary_arithmetic_expression() {
	int ret = NOTFOUND;
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.get_int) {
		case MULT:
		case DIV:
			syn_printe();
			break;
		case PLUS:gen_incode("+");
			break;
		case MINUS:gen_incode("-");
			break;
		}
		match(lookahead.code, lookahead.attribute.arr_op);
		ret = primary_arithmetic_expression();
		break;
	default:
		syn_printe();
	}
	return;
}

/*
<primary arithmetic expression> ->
<arithmetic variable identifier>
| <floating point literal>
| <integer literal>
| (<arithmetic expression>)

FIRST SET = { AVID, FPL, INL, '(' }

Author: John Pilon
*/
int primary_arithmetic_expression() {
	unsigned char ret = NOTFOUND;
	Token* token;
	char * temp[30];
	switch (lookahead.code) {
	case AVID_T:
		ret = t_locate(*sc_st, lookahead.attribute.vid_lex, &token);
		if (ret == NOTFOUND)
		{
			lookahead.type = INTEGER;
			table_adds(sc_st, lookahead);
		}
		gen_incode(lookahead.attribute.vid_lex);
		match(lookahead.code, lookahead.attribute.vid_lex);
		return ret;
	case INL_T:
		sprintf(temp, "%d", lookahead.attribute.int_value);
		gen_incode(temp);
		match(lookahead.code, lookahead.attribute.int_value);
		ret = INTEGER;
		break;
	case FPL_T:
		sprintf(temp, "%.4g", lookahead.attribute.flt_value);
		gen_incode(temp);
		match(lookahead.code, lookahead.attribute.flt_value);
		ret = FLOAT;
		break;
	case LPR_T:
		gen_incode("(");
		match(lookahead.code, lookahead.attribute.arr_op);
		ret = arithmetic_expression();
		gen_incode(")");
		match(RPR_T, NO_ATTR);
		break;
	default:
		syn_printe();
	}
	return ret;
}

/*
<string expression> ->
<primary string expression> <string expression'>

FIRST_SET = { SVID, STL }

Author: John Pilon
*/
void string_expression(){
	primary_string_expression();
	string_expression_p();
}

/*
<string expression'> ->
 #<primary string expression> <string expression'> | E

FIRST SET = { #, E }

Author: John Pilon
*/
void string_expression_p(){
	if(lookahead.code == SCC_OP_T){
		match(SCC_OP_T, NO_ATTR);
		primary_string_expression();
		string_expression_p();
	}
}

/*
<primary string expression> ->
<string variable identifier>
| <string literal>

FIRST_SET = { SVID, STL }

Author: Daniel Brenot
*/
void primary_string_expression()
{
	int strSize = 0, i = 0;
	char * temp;
	switch (lookahead.code)
	{
	case SVID_T:
		gen_incode(lookahead.attribute.vid_lex);
		match(lookahead.code, NO_ATTR);
		break;
	case STR_T:
		b_mark(str_LTBL, lookahead.attribute.str_offset);
		b_reset(str_LTBL);
		while (b_getc(str_LTBL) != '\0') { strSize++; }
		temp = (char*)malloc(sizeof(char)*(strSize + 1));
		b_reset(str_LTBL);
		while (strSize >= 0) { temp[i] = b_getc(str_LTBL); strSize--; i++; }
		gen_incode("\"");
		gen_incode(temp);
		gen_incode("\"");
		match(lookahead.code, NO_ATTR);
		break;
	default:
		syn_printe();
	}
}

/*
<pre-condition> ->
TRUE | FALSE

FIRST SET = { TRUE, FALSE }

Author: Daniel Brenot
*/
void pre_condition() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.get_int) {
		case TRUE:
			gen_incode("((");
			match(KW_T, TRUE);
			return;
		case FALSE:
			gen_incode("(!(");
			match(KW_T, FALSE);
			return;
		default:
			break;
		}
	default:
		syn_printe();
		break;
	}

}

/*
<conditional expression> ->
<logical OR  expression>

FIRST_SET = { AVID, SVID, FPL, INL, STL }

Author: Daniel Brenot
*/
void conditional_expression(){
	logical_OR_expression();
}

/*
<logical OR expression> ->
<logical AND expression> <logical OR expression'>

FIRST SET = { AVID, SVID, FPL, INL, STL }

Author: Daniel Brenot
*/
void logical_OR_expression(){
	logical_AND_expression();
	logical_OR_expression_p();
}

/*
<logical OR expression'> ->
.OR. <logical AND expression> <logical OR expression'>

FIRST SET = { .OR., AVID, SVID, FPL, INL, STL }

Author: Daniel Brenot
*/
void logical_OR_expression_p() {
	if (lookahead.code != LOG_OP_T || lookahead.attribute.log_op != OR) return;
	gen_incode("||");
	match(LOG_OP_T, OR);
	logical_AND_expression();
	logical_OR_expression_p();
}
/*
<logical AND expression> ->
<relational expression> <logical AND expression'>

FIRST SET = { AVID, SVID, FPL, INL, STL }

Author: Daniel Brenot
*/
void logical_AND_expression(){
	relational_expression();
	logical_AND_expression_p();	
}

/*
<logical AND expression'> ->
.AND. <relational expression> <logical AND expression'>

FIRST SET = { .AND., AVID, SVID, FPL, INL, STL }

Author: John Pilon
*/
void logical_AND_expression_p() {
	if (lookahead.code != LOG_OP_T || lookahead.attribute.log_op != AND)return;
	gen_incode("&&");
	match(LOG_OP_T, AND);
	relational_expression();
	logical_AND_expression_p();
}

/*
<primary a_relational expression> ->
<variable identifier>
| <floating-point literal>
| <integer literal>

FIRST SET = { AVID, FPL, INL }

Author: John Pilon
*/
void primary_a_relational_expression() {
	char temp[30];
	switch (lookahead.code) {
	case AVID_T:
		gen_incode(lookahead.attribute.vid_lex);
		match(lookahead.code, NO_ATTR);
		break;
	case FPL_T:
		sprintf(temp, "%.4g", lookahead.attribute.flt_value);
		gen_incode(temp);
		match(lookahead.code, NO_ATTR);
		break;
	case INL_T:
		sprintf(temp, "%d", lookahead.attribute.int_value);
		gen_incode(temp);
		match(lookahead.code, NO_ATTR);
		break;
	default:
		syn_printe();
	}
}


/*
<primary s_relational expression> ->
<string variable identifier> | <string literal>

FIRST_SET = { SVID, STL }

Author: John Pilon
*/
void primary_s_relational_expression(){
	primary_string_expression();
}

/*
<relational expression> ->
<primary a_relational expression> <primary a_relational expression'>
| <primary s_relational expression> <primary s_relational expression'>

FIRST SET = { AVID, SVID }

Author: Daniel Brenot
*/
void relational_expression() {
	switch (lookahead.code) {
	case AVID_T:
	case FPL_T:
	case INL_T:
		primary_a_relational_expression();
		switch (lookahead.attribute.int_value)
		{
		case EQ:gen_incode("=="); break;
		case NE:gen_incode("!="); break;
		case GT:gen_incode(">"); break;
		case LT:gen_incode("<"); break;
		}
		match(REL_OP_T, lookahead.attribute.rel_op);
		primary_a_relational_expression();
		break;
	case SVID_T:
	case STR_T:
		primary_s_relational_expression();
		match(REL_OP_T, lookahead.attribute.rel_op);
		primary_s_relational_expression();
		break;

	default:
		syn_printe();
	}
}

/*
<relational expression'> ->
== <primary a_relational expression>
| <> <primary a_relational expression>
| > <primary a_relational expression>
| < <primary a_relational expression>

FIRST_SET = { ==, <>, >, <, AVID }

Author: John Pilon
*/
void relational_expression_p() {
	if (lookahead.code == REL_OP_T)
	{
		switch (lookahead.attribute.rel_op)
		{
		case EQ:gen_incode("==");
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		case NE:gen_incode("!=");
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		case GT:gen_incode(">");
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		case LT:gen_incode("<");
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		}
	}
	syn_printe();
}

/*
<relational expression_s'> ->
== <primary s_relational expression>
| <> <primary s_relational expression>
| > <primary s_relational expression>
| < <primary s_relational expression>

FIRST SET = { ==, <>, >, <, SVID }

Author: John Pilon
*/
void relational_expression_s_p()
{
	if (lookahead.code == REL_OP_T)
	{
		switch (lookahead.attribute.rel_op)
		{
		case EQ:gen_incode("==");
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		case NE:gen_incode("!=");
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		case GT:gen_incode(">");
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		case LT:gen_incode("<");
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		}
	}
	syn_printe();
}

/*
<variable identifier> ->
<arithmetic variable identifier> | <string variable identifier

FIRST SET = { AVID, SVID }

Author: John Pilon
*/
void variable_identifier()
{
	switch (lookahead.code)
	{
	case AVID_T:
	case SVID_T:
		appendToken();
		match(lookahead.code, NO_ATTR);
		break;
	default:
		syn_printe();
	}
}

/*
<variable list> -> <variable identifier>, <variable list’>

FIRST SET = { AVID, SVID, ',' }

Author: John Pilon
*/
void variable_list() {
	clearList(tokenList);
	variable_identifier();
	variable_list_p();
}


/*
<variable list’> -> <variable identifier>, <variable list’> | E

FIRST SET = { AVID, SVID, E, ',' } 

Author: John Pilon
*/
void variable_list_p() {
	if (lookahead.code != COM_T) return;
	match(COM_T, NO_ATTR);
	variable_identifier();
	variable_list_p();
}

void appendToken() {
	LinkedList* iterator;
	LinkedList* newLink = malloc(sizeof(LinkedList));
	newLink->next = NULL;
	newLink->current = malloc(sizeof(Token));
	strcpy(newLink->current->attribute.vid_lex, lookahead.attribute.vid_lex);
	newLink->current->type = lookahead.type;
	iterator = tokenList;
	if (iterator)
	{
		while (iterator->next)
		{
			iterator = iterator->next;
		}
		iterator->next = newLink;
	}
	else
	{
		tokenList = newLink;
	}
}
void clearList(LinkedList* list) {
	LinkedList* iterator = list;
	LinkedList* temp;
	while (iterator)
	{
		temp = iterator;
		iterator = iterator->next;
		free(temp);
	}
	tokenList = NULL;
}