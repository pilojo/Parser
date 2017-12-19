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
	Symbol *symbolArry;
	table = (pSymbolTable)calloc(1, sizeof(SymbolTable));
	if (table == NULL) return NULL;
	symbolArry = (Symbol*)malloc(init_capacity * sizeof(Symbol));
	if (!symbolArry)
	{
		free(table);
		return NULL;
	}
	table->tb_head = symbolArry;
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
pSymbolTable table_adds(pSymbolTable const pST, Symbol symbol) {
	/* check for valid parameters */
	if (!pST) return NULL;
	/* Check if there's room for a symbol */
	if (pST->adds_offset < pST->capacity) {
		pST->tb_head[pST->adds_offset++] = symbol;
	}
	else return NULL;
	return pST;
}

/* Purpose: Finds a symbol in the symbol table starting from the most recent variables
** Author: Daniel Brenot
** Version: 1.0
** Called functions:
** Parameters: pST: a valid Symbol Table pointer, name: a char array to be found
** Return Value: Symbol Table pointer, NULL on fail
** Algorithm: Check if there's room for a symbol, if not return null (This symbol table implementation is not dynamic)
*/
unsigned char t_locate(SymbolTable const pST, char * name)
{
	unsigned short i;
	for (i = pST.adds_offset; i >=0; i--)
	{
		if (strcmp(name, pST.tb_head[i].name)==0)
		{
			return pST.tb_head[i].type;
		}
	}
	return NOTFOUND;
}