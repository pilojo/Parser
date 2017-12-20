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

void parser(Buffer* in_buf){
	sc_buf = in_buf;
	lookahead = malar_next_token(sc_buf);
	program(); match(SEOF_T, NO_ATTR);
	gen_incode("PLATY: Source file parsed");
}

void gen_incode(char* message){
	printf("%s\n", message);
}

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

/*
   <program>  ->
    PLATYPUS {<opt_statements>}
   FIRST SET = { PLATYPUS }
*/
void program(){
	match(KW_T, PLATYPUS); match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	gen_incode("PLATY: Program parsed");
}

/*
   <opt_statements> ->
    <statements> | E
   FIRST SET = { AVID_T, SVID_T, IF, WHILE, READ, WRITE, E }
*/
void opt_statements(){
	
	switch(lookahead.code){
		
		case KW_T:
			
			switch(lookahead.attribute.get_int){
				
				case PLATYPUS:case ELSE:case THEN:case REPEAT:
					
					gen_incode("PLATY: Opt_statements parsed");
					return;
					
				default:
					break;
			}
			
		case AVID_T:case SVID_T:
			statements();
			break;
			
		default:
			gen_incode("PLATY: Opt_statements parsed");
	}
}

void statements(){
	statement();
	statements_p();
}

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

void assignment_statement(){
	assignment_expression();
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Assignment statement parsed");	
}

void assignment_expression(){
	switch(lookahead.code){
		case AVID_T:
			match(AVID_T, NO_ATTR);
			match(ASS_OP_T, NO_ATTR);
			arithmetic_expression();
			gen_incode("PLATY: Assignment expression (arithmetic) parsed");
			break;
		case SVID_T:
			match(SVID_T, NO_ATTR);
			match(ASS_OP_T, NO_ATTR);
			string_expression();
			gen_incode("PLATY: Assignment expression (string) parsed");
			break;
		default:
			syn_printe();
			return;
	}
}

void iteration_statement(){
	match(KW_T, WHILE); pre_condition();
	match(LPR_T, NO_ATTR); conditional_expression(); match(RPR_T, NO_ATTR);
	match(KW_T, REPEAT); match(LBR_T, NO_ATTR); statements(); match(RBR_T, NO_ATTR); match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Iteration statement parsed");
}

void selection_statement(){
		match(KW_T, IF); match(KW_T, TRUE); match(LPR_T, NO_ATTR); conditional_expression(); match(RPR_T, NO_ATTR);
		match(KW_T, THEN); match(LBR_T, NO_ATTR); opt_statements(); match(RBR_T, NO_ATTR);
		match(KW_T, ELSE); match(LBR_T, NO_ATTR); opt_statements(); match(RBR_T, NO_ATTR); match(EOS_T, NO_ATTR);
		gen_incode("PLATY: Selection statement parsed");
}

void read_statement(){
	match(KW_T, READ); match(LPR_T, NO_ATTR); variable_list(); match(RPR_T, NO_ATTR); match(EOS_T, NO_ATTR);	
	gen_incode("PLATY: Input statement parsed");
}

void write_statement() {
	match(KW_T, WRITE); match(LPR_T, NO_ATTR);
	switch (lookahead.code) {
	case AVID_T: case SVID_T:
		variable_list();
		break;
	case STR_T:
		match(STR_T, NO_ATTR);
		gen_incode("PLATY: Output list (string literal) parsed");
		break;
	default:
		gen_incode("PLATY: Output list (empty) parsed");
	}
	match(RPR_T, NO_ATTR); match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Output statement parsed");
}


void arithmetic_expression(){
	switch(lookahead.code){
	case ART_OP_T:
		switch (lookahead.attribute.arr_op) {
		case MULT:case DIV:
			syn_printe();
			return;
		}
		unary_arithmetic_expression();
		break;
	case AVID_T: case FPL_T: case INL_T: case LPR_T:
		additive_arithmetic_expression();
		break;
	case EOS_T:
		return;
	default:
		syn_printe();
	}
	gen_incode("PLATY: Arithmetic expression parsed");
}

void additive_arithmetic_expression(){
	multiplicative_arithmetic_expression();
	additive_arithmetic_expression_p();
}

void additive_arithmetic_expression_p()
{
	if (lookahead.code == ART_OP_T &&lookahead.attribute.arr_op != MULT && lookahead.attribute.arr_op != DIV)
	{
		match(lookahead.code, lookahead.attribute.arr_op);
		multiplicative_arithmetic_expression();
		additive_arithmetic_expression_p();
		gen_incode("PLATY: Additive arithmetic expression parsed");
	}
}

void multiplicative_arithmetic_expression(){
	primary_arithmetic_expression();
	multiplicative_arithmetic_expression_p();
}

void multiplicative_arithmetic_expression_p(){
	if (lookahead.code == ART_OP_T &&lookahead.attribute.arr_op != PLUS &&lookahead.attribute.arr_op != MINUS)
	{
		match(lookahead.code, lookahead.attribute.arr_op);
		primary_arithmetic_expression();
		multiplicative_arithmetic_expression_p();
		gen_incode("PLATY: Multiplicative arithmetic expression parsed");
	}
}

void unary_arithmetic_expression(){
	switch(lookahead.code){
		case ART_OP_T:
			switch (lookahead.attribute.get_int) {
			case MULT: case DIV:
				syn_printe();
				return;
			}
			match(lookahead.code, lookahead.attribute.arr_op);
			primary_arithmetic_expression();
			gen_incode("PLATY: Unary arithmetic expression parsed");
			break;
		default:
			syn_printe();
	}
}


void primary_arithmetic_expression(){
	switch(lookahead.code){
	case AVID_T: case FPL_T: case INL_T:
		match(lookahead.code, lookahead.attribute.arr_op);
		break;
	case LPR_T:
		match(lookahead.code, lookahead.attribute.arr_op);
		arithmetic_expression();
		match(RPR_T, NO_ATTR);
		break;
	default:
			syn_printe();
	}
	gen_incode("PLATY: Primary arithmetic expression parsed");
}
//daniel
void string_expression(){
	primary_string_expression();
	string_expression_p();
	gen_incode("PLATY: String expression parsed");
}

void string_expression_p(){
	if(lookahead.code == SCC_OP_T){
		match(SCC_OP_T, NO_ATTR);
		primary_string_expression();
		string_expression_p();
	}
}

void primary_string_expression()
{
	switch (lookahead.code)
	{
	case SVID_T: case STR_T:
		match(lookahead.code, NO_ATTR);
		break;
	default:
		syn_printe();
	}
	gen_incode("PLATY: Primary string expression parsed");
}

void pre_condition(){
	switch(lookahead.code){
		case KW_T:
			switch(lookahead.attribute.get_int){
				case TRUE: 
					match(KW_T, TRUE);
					return;
				case FALSE:
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
//daniel
void conditional_expression(){
	logical_OR_expression();
	gen_incode("PLATY: Conditional expression parsed");
}

void logical_OR_expression(){
	logical_AND_expression();
	logical_OR_expression_p();
}

void logical_OR_expression_p(){
	if (lookahead.code != LOG_OP_T || lookahead.attribute.log_op != OR) return;
	match(LOG_OP_T, OR);
	logical_AND_expression();
	logical_OR_expression_p();
	gen_incode("PLATY: Logical OR expression parsed");
}

void logical_AND_expression(){
	relational_expression();
	logical_AND_expression_p();	
}

void logical_AND_expression_p(){
	if (lookahead.code != LOG_OP_T || lookahead.attribute.log_op != AND)return;
	match(LOG_OP_T, AND);
	relational_expression();
	logical_AND_expression_p();
	gen_incode("PLATY: Logical AND expression parsed");
}

void primary_a_relational_expression(){
	switch(lookahead.code){
	case AVID_T: case FPL_T: case INL_T:
		match(lookahead.code, NO_ATTR);
		break;
	default:
		syn_printe();
	}
	gen_incode("PLATY: Primary a_relational expression parsed");
}

void primary_s_relational_expression(){
	primary_string_expression();
	gen_incode("PLATY: Primary s_relational expression parsed");
}

//daniel
void relational_expression() {
	switch(lookahead.code){
	case AVID_T: case FPL_T: case INL_T:
			primary_a_relational_expression();
			match(REL_OP_T, lookahead.attribute.rel_op);
			primary_a_relational_expression();
			break;
	case SVID_T: case STR_T:
			primary_s_relational_expression();
			match(REL_OP_T, lookahead.attribute.rel_op);
			primary_s_relational_expression();
			break;

	default:
		syn_printe();
	}
	gen_incode("PLATY: Relational expression parsed");
}

void relational_expression_p(){
	if (lookahead.code == REL_OP_T)
	{
		switch (lookahead.attribute.rel_op)
		{
		case EQ:case NE:case GT:case LT:
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		}
	}
	syn_printe();
}

void relational_expression_s_p()
{
	if (lookahead.code == REL_OP_T)
	{
		switch (lookahead.attribute.rel_op)
		{
		case EQ:case NE:case GT:case LT:
			match(lookahead.code, lookahead.attribute.arr_op);
			return;
		}
	}
	syn_printe();
}

void variable_identifier()
{
	switch (lookahead.code)
	{
	case AVID_T:case SVID_T:
		match(lookahead.code, NO_ATTR);
		break;
	default:
		syn_printe();
	}
}

void variable_list(){
	variable_identifier();
	variable_list_p();
	gen_incode("PLATY: Variable list parsed");
}

void variable_list_p(){
	if (lookahead.code != COM_T) return;
		match(COM_T, NO_ATTR);
		variable_identifier();
		variable_list_p();
}


