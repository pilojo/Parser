/*
File name : token.h
Compiler : MS Visual Studio 2015
Author : John Pilon, 040822687; Daniel Brenot, 040837035
Course : CST 8152 – Compilers, Lab Section : 12
Assignment : 3
Date : January 4th, 2018
Professor : Sv.Ranev
Purpose : Header to define Tokens
*/
#ifndef TOKEN_H_
#define TOKEN_H_

#pragma warning(1:4001) /*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

							   /* Constants */

#define VID_LEN 8   /* variable identifier length */
#define ERR_LEN 20  /* error message length */
#define INL_LEN 5   /* maximum number of digits for IL */

							   /* Token codes */

#define ERR_T     0  /* Error token */
#define SEOF_T    1  /* Source end-of-file token */
#define AVID_T    2  /* Arithmetic Variable identifier token */
#define SVID_T    3  /* String Variable identifier token */
#define FPL_T     4  /* Floating point literal token */
#define INL_T     5  /* Integer literal token */
#define STR_T     6  /* String literal token */
#define SCC_OP_T  7  /* String concatenation operator token */
#define ASS_OP_T  8  /* Assignment operator token */
#define ART_OP_T  9  /* Arithmetic operator token */
#define REL_OP_T 10  /* Relational operator token */ 
#define LOG_OP_T 11  /* Logical operator token */
#define LPR_T    12  /* Left parenthesis token */
#define RPR_T    13  /* Right parenthesis token */
#define LBR_T    14  /* Left brace token */
#define RBR_T    15  /* Right brace token */
#define KW_T     16  /* Keyword token */
#define COM_T    17  /* Comma token */
#define EOS_T    18  /* End of statement *(semi - colon) */

#define NOTFOUND 0
#define INTEGER 1
#define FLOAT 2
#define STRING 3
							   /* Operators token attributes */

typedef enum ArithmeticOperators { PLUS, MINUS, MULT, DIV } Arr_Op;
typedef enum RelationalOperators { EQ, NE, GT, LT } Rel_Op;
typedef enum LogicalOperators { AND, OR } Log_Op;


/* Structure declaring the token and its attributes */

typedef union TokenAttribute {
	int get_int;      /* integer attributes accessor */
	Arr_Op arr_op;    /* arithmetic operator attribute code */
	Rel_Op rel_op;    /* relational operator attribute code */
	Log_Op log_op;    /* logical operator attribute code */
	int int_value;    /* integer literal attribute (value) */
	int kwt_idx;      /* keyword index in the keyword table */
	short str_offset; /* sring literal offset from the beginning of */
					  /* the string literal buffer (str_LTBL->cb_head) */
	float flt_value;    /* floating-point literal attribute (value) */
	char vid_lex[VID_LEN + 1]; /* variable identifier token attribute */
	char err_lex[ERR_LEN + 1]; /* error token attribite */
} TA;

typedef struct Token
{
	int code;                 /* token code */
	unsigned char type; /*VID Type*/
	unsigned int line; /*The line the token existed on*/
	TA attribute; /* token attribute */
} Token;

#endif
