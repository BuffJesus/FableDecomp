#include "fable_string_parser.h"
#include <stdio.h>
#include <string.h>
static int assignments;
CCharString& CCharString::operator=(const CCharString& other)
{
    *reinterpret_cast<unsigned long*>(this)=*reinterpret_cast<const unsigned long*>(&other);
    ++assignments;return *this;
}
static void FABLE_FASTCALL error(const CCharString&) {}
int main()
{
    unsigned long storage[10];
    CStringParser* parser=reinterpret_cast<CStringParser*>(storage);
    char source[]="enum Example";unsigned long filename=0x11223344;
    CCharString& name=*reinterpret_cast<CCharString*>(&filename);
    memset(storage,0x5a,sizeof(storage));
    parser->Init(source,error,name,19);
    if(parser->String!=source || parser->StringPos!=0 || parser->Line!=19 || parser->BaseLineOffset!=19 || parser->ErrorCallback!=error || parser->InQuotes || assignments!=1)return 1;
    if(storage[0]!=0x5a5a5a5a || storage[3]!=filename || storage[4]!=0x5a5a5a5a || storage[5]!=0x5a5a5a5a || (storage[7]&0xffffff00)!=0x5a5a5a00)return 2;
    parser->Init(source,0,name,0);
    if(parser->ErrorCallback || parser->Line || parser->BaseLineOffset || assignments!=2)return 3;
    puts("PARSER_INIT_TYPED_OK cases=2");return 0;
}
