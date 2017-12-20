#include "symbol_table.h"
//#define ST_TEST

#ifdef ST_TEST
int main(int argc, char ** argv)
{
	unsigned char result;
	//holds the temporary user input to be discarded
	char temp;
	
	SymbolTable * st;
	//Allocates a symbol table with size 1000
	st = t_allocate(1000);

	//Creates test symbols
	Symbol integerSymbol;
	Symbol floatSymbol;
	Symbol stringSymbol;

	//Puts symbol name into symbol
	strncpy(integerSymbol.name,"integer", 8);
	strncpy(floatSymbol.name, "float", 6);
	strncpy(stringSymbol.name, "string", 7);

	//Sets type of symbol
	integerSymbol.type = INTEGER;
	floatSymbol.type = FLOAT;
	stringSymbol.type = STRING;
	
	//Adds symbol to symbol table
	table_adds(st, integerSymbol);
	table_adds(st, floatSymbol);
	table_adds(st, stringSymbol);


	result = t_locate(*st, "integer");
	printf("Integer result: %d\n", result);
	result = t_locate(*st, "float");
	printf("Float result: %d\n", result);
	result = t_locate(*st, "string");
	printf("String result: %d\n", result);
	scanf("%c",&temp);
	return 0;
}
#endif
