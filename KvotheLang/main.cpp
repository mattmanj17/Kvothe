#include "pool.h"
#include "regexparse.h"
#include "nfa.h"
#include "dfa.h"

int main()
{	
	const char * pChzFileName = "example.regex";
	FILE * pFile = fopen(pChzFileName, "r");
			
	//printf("regex ast:\n\n");
	
	CRegexParser parser;
	parser.ParseFile(pFile);
	//parser.PRegexAstParsed()->PrintDebug();

	fclose(pFile);

	//printf("\n\nNFA:\n\n");

	CNfa nfa;
	nfa.Build(parser.PRegexAstParsed());
	//nfa.PrintDebug();

	//printf("\nDFA:\n\n");

	CDfa dfa;
	dfa.Build(&nfa);
	//dfa.PrintDebug();

	return 0;
}