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
	gen_incode("PLATY: Sourse file parsed");
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

void syn_printe(){
	
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
			gen_incode("Platy: Opt_statements parsed");
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
				default:
					break;
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
					return;
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
			gen_incode("PLATY: Assignment expression(arithmetic) parsed");
			break;
		case SVID_T:
			match(SVID_T, NO_ATTR);
			match(ASS_OP_T, NO_ATTR);
			string_expression();
			gen_incode("PLATY: Assignment expression(string) parsed");
			break;
		default:
			syn_printe();
			return;
	}
}

void iteration_statement(){
	match(KW_T, WHILE); pre_condition();
	match(LPR_T, NO_ATTR); conditional_expression(); match(RPR_T, NO_ATTR);
	match(KW_T, REPEAT); match(LBR_T, NO_ATTR); opt_statements(); match(RBR_T, NO_ATTR); match(EOS_T, NO_ATTR);
	gen_incode("PLATY: WHILE statement parsed");
}

void selection_statement(){
		match(KW_T, IF); match(KW_T, TRUE); match(LPR_T, NO_ATTR); conditional_expression(); match(RPR_T, NO_ATTR);
		match(KW_T, THEN); match(LBR_T, NO_ATTR); opt_statements(); match(RBR_T, NO_ATTR);
		match(KW_T, ELSE); match(LBR_T, NO_ATTR); opt_statements(); match(RBR_T, NO_ATTR); match(EOS_T, NO_ATTR);
}

void read_statement(){
	match(KW_T, READ); match(LPR_T, NO_ATTR); variable_list(); match(RPR_T, NO_ATTR); match(EOS_T, NO_ATTR);	
	gen_incode("PLATY: READ statement parsed");
}

void write_statement(){
	match(KW_T, WRITE); match(LPR_T, NO_ATTR); variable_list(); match(RPR_T, NO_ATTR); match(EOS_T, NO_ATTR);	
	gen_incode("PLATY: WRITE statement parsed");
}

void arithmetic_expression(){
	
}

void string_expression(){
	
}

void pre_condition(){
	switch(lookahead.code){
		case KW_T:
			switch(lookahead.attribute.get_int){
				case TRUE: 
					match(KW_T, TRUE);
					gen_incode("Platy: pre-condition parsed");
				case FALSE:
					match(KW_T, FALSE);
					gen_incode("PLATY: pre-condition parsed");
					return;
				default:
					break;
			}
		default:
			syn_printe();
			break;
	}
	
}

void conditional_expression(){
	
}

void variable_list(){
	switch(lookahead.code){
		case AVID_T:
			match(AVID_T, NO_ATTR);
			match(COM_T, NO_ATTR);
			variable_list_p();
			break;
		case SVID_T:
			match(SVID_T, NO_ATTR);
			match(COM_T, NO_ATTR);
			variable_list_p();
			break;
		default:
			syn_printe();
			break;
	}
}

void variable_list_p(){
	switch(lookahead.code){
		case AVID_T:
			match(AVID_T, NO_ATTR);
			match(COM_T, NO_ATTR);
			variable_list_p();
			break;
		case SVID_T:
			match(SVID_T, NO_ATTR);
			match(COM_T, NO_ATTR);
			variable_list_p();
			break;
		default:
			break;	
	}
}


