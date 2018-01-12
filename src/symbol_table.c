#include "symbol_table.h"

/* Purpose: Allocate memory a symbol table
** Author: Daniel Brenot
** Version: 1.0
** Called functions: malloc(), calloc(), free()
** Parameters: init_capacity(in bytes): 0..(MAX_CAPACITY);
** Algorithm: Allocate memory for the symbol table descriptor and symbol array, return symbol table descriptor
*/
SymbolTable *t_allocate(unsigned short init_capacity)
{
	SymbolTable * table;
	Token *symbolArry;
	table = (pSymbolTable)calloc(1, sizeof(SymbolTable));
	if (table == NULL) return NULL;
	symbolArry = (Token*)malloc(init_capacity * sizeof(Token));
	if (!symbolArry)
	{
		free(table);
		return NULL;
	}
	table->tb_head = symbolArry;
	table->adds_offset = 0;
	table->capacity = init_capacity;
	return table;
}

/* Purpose: Adds a symbol to the symbol table
** Author: Daniel Brenot
** Version: 1.0
** Called functions:
** Parameters: pST: a valid Symbol Table pointer, symbol: any symbol
** Return Value: Symbol Table pointer, NULL on fail
** Algorithm: Check if there's room for a symbol, if not return null (This symbol table implementation is not dynamic)
*/
pSymbolTable table_adds(pSymbolTable const pST, Token symbol) {
	/* check for valid parameters */
	if (!pST) return NULL;
	/* Check if there's room for a symbol */
	if (pST->adds_offset < pST->capacity) {
		pST->tb_head[pST->adds_offset].attribute = symbol.attribute;
		pST->tb_head[pST->adds_offset].code = symbol.code;
		pST->tb_head[pST->adds_offset].line = symbol.line;
		pST->tb_head[pST->adds_offset].type = symbol.type;
		pST->adds_offset++;
	}
	else return NULL;
	return pST;
}

/* Purpose: Finds a symbol in the symbol table starting from the most recent variables
** Author: Daniel Brenot
** Version: 1.0
** Called functions:
** Parameters: pST: a valid Symbol Table pointer, name: a char array to be found, token: a pointer to a pointer of a token to be set to the token found
** Return Value: Symbol Table pointer, NULL on fail
** Algorithm: Check if there's room for a symbol, if not return null (This symbol table implementation is not dynamic)
*/
unsigned char t_locate(SymbolTable const pST, char * name, Token* token)
{
	short i;
	for (i = pST.adds_offset-1; i >=0; i--)
	{
		if (strcmp(name, pST.tb_head[i].attribute.vid_lex)==0)
		{
			token = &pST.tb_head[i];
			return pST.tb_head[i].type;
		}
	}
	return NOTFOUND;
}

void t_print(SymbolTable const pST)
{
	short i;
	Token * token;
	token = t_locate(pST, "z$", &token);
	for (i = pST.adds_offset - 1; i >= 0; i--)
	{
		printf("Symbol Record:\n\tAttribute:%s\n\tType:%i\n\tLine%i\n",pST.tb_head[i].attribute.vid_lex,pST.tb_head[i].type,pST.tb_head[i].line);
	}
}

void t_free(SymbolTable const pST)
{
	short i;
	for (i = pST.adds_offset - 1; i >= 0; i--)
	{
		free(pST.tb_head);
	}
}