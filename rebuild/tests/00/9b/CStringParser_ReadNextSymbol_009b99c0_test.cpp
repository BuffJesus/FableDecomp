#include "fable_string_parser.h"
#include <stdio.h>
static char recorded;static int calls;
void CCharString::SetAsSubString(const char* text,long length){recorded=*text;calls+=length==1?1:100;}
struct View:CStringParser { static bool read(CStringParser* p,CParsedItem& item){return static_cast<View*>(p)->ReadNextSymbol(item);} };
int main(){char* sources[]={"", ";", "'", "\"", "+"};
 for(unsigned i=0;i<5;++i){unsigned long raw[10]={0},itemRaw[3]={0};CStringParser* p=reinterpret_cast<CStringParser*>(raw);CParsedItem& item=*reinterpret_cast<CParsedItem*>(itemRaw);p->String=sources[i];p->Line=23;calls=0;bool result=View::read(p,item);
 if(result!=(i!=0)||p->StringPos!=(i?1:0)||calls!=(i?1:0)||p->Line!=23)return 1;
 if(i && (item.Type!=CParsedItem::SYMBOL || recorded!=sources[i][0] || p->InQuotes!=(i==2||i==3)))return 2;}
 puts("PARSER_SYMBOL_OK cases=5");return 0;}
