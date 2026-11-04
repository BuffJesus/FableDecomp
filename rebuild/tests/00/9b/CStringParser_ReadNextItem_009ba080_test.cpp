#include "fable_string_parser.h"
#include <stdio.h>
#include <string.h>
static int route,skips;static bool good=true;
long CStringParser::SkipWhitespace(){++skips;while(String[StringPos]==' ')++StringPos;return 0;}
bool CStringParser::ReadNextNumber(CParsedItem&){route=1;return true;}
bool CStringParser::ReadNextString(CParsedItem&){route=2;return true;}
bool CStringParser::ReadNextSymbol(CParsedItem&){route=3;return false;}
int main(){struct Case{char* text;int expected;}cases[]={{"12",1},{" -2",1},{"-a",3},{"+2",3},{".5",3},{"name",2},{"_id",2},{"",3},{"'quoted'",3}};
 for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i){unsigned long raw[10]={0},itemRaw[3]={0};CStringParser* parser=reinterpret_cast<CStringParser*>(raw);parser->String=cases[i].text;route=skips=0;bool result=parser->ReadNextItem(*reinterpret_cast<CParsedItem*>(itemRaw));if(route!=cases[i].expected || skips!=1 || result!=(route!=3))return 1;}
 puts("PARSER_NEXT_ITEM_OK cases=9");return 0;}
