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
static int integers,floats;static bool good=true;
CCharString FABLE_FASTCALL GFIntToCharString(long value){++integers;good &= value==-17;return "integer";}
CCharString FABLE_FASTCALL GFFloatToCharString(double value,long precision){++floats;good &= value==1.25 && precision==6;return "float";}
int main(){for(unsigned kind=0;kind<5;++kind){CParsedItem item;item.Type=static_cast<EParsedItemType>(kind);slot(item.String)=copy("name");if(kind==3)item.Value.Integer=-17;else item.Value.Float=1.25f;CCharString result=item.ConvertToString();const char* expected=kind==3?"integer":kind==4?"float":kind?"name":"NULL";if(strcmp(text(result),expected)||!good)return 1;}
 if(integers!=1||floats!=1)return 2;puts("PARSED_CONVERT_STRING_OK cases=5");return 0;}
