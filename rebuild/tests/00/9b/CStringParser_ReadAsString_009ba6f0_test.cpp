#include "fable_string_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char*& slot(const CCharString& s){return *reinterpret_cast<char**>(const_cast<CCharString*>(&s));}
static const char* text(const CCharString& s){return slot(s)?slot(s):"";}
static char* copy(const char* s){char* p=(char*)malloc(strlen(s)+1);strcpy(p,s);return p;}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* s,long):storage_(0){slot(*this)=copy(s);}
CCharString::CCharString(const CCharString& s):storage_(0){slot(*this)=copy(text(s));}
CCharString::~CCharString(){free(slot(*this));}
static int mode,errors;static bool good=true;
bool CStringParser::ReadNextItem(CParsedItem& item){item.Type=mode==1?CParsedItem::INTEGER:CParsedItem::STRING;slot(item.String)=copy("CREATURE");return mode!=2;}
void CStringParser::Error(const CCharString& message){++errors;good &= !strcmp(text(message),"Error parsing string");}
int main(){unsigned long raw[10]={0};CStringParser* p=reinterpret_cast<CStringParser*>(raw);
 for(mode=0;mode<3;++mode){errors=0;CCharString result=p->ReadAsString();if(strcmp(text(result),mode?"":"CREATURE") || errors!=(mode?1:0) || !good)return 1;}
 puts("PARSER_READ_STRING_OK cases=3");return 0;}
