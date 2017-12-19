#ifndef  SYMBOL_TABLE_H_
#define  SYMBOL_TABLE_H_ 


#include <stdio.h>  /* standard input/output */
#include <string.h>
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

#define NOTFOUND 0
#define INTEGER 1
#define FLOAT 2
#define STRING 3


typedef struct SymbolDescriptor {
	char name[9];
	unsigned char type;
} Symbol;

typedef struct SymbolTableDescriptor {
	Symbol *tb_head;   /* pointer to the beginning of symbol array (symbol array) */
	unsigned short capacity;    /* current dynamic memory size (in bytes) allocated to token buffer */
	unsigned short adds_offset;  /* the offset (in chars) to the add-token location */
} SymbolTable, *pSymbolTable;


pSymbolTable table_adds(pSymbolTable const pST, Symbol symbol);
SymbolTable *t_allocate(unsigned short init_capacity);
unsigned char t_locate(SymbolTable const pST, char * name);
#endif