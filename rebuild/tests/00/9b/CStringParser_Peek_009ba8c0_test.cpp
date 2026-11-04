#include "fable_string_parser.h"
#include <stdio.h>
CCharString::CCharString():storage_(0){}
CCharString::~CCharString(){}
static bool success;
bool CStringParser::ReadNextItem(CParsedItem& item){unsigned long* cursor=reinterpret_cast<unsigned long*>(&InQuotes);cursor[0]=0x76543200;StringPos=90;Line=80;item.Type=CParsedItem::STRING;return success;}
int main(){for(unsigned test=0;test<4;++test){unsigned long raw[10]={0},itemRaw[3]={0};CStringParser* p=reinterpret_cast<CStringParser*>(raw);raw[7]=0x12345601;raw[8]=12;raw[9]=34;success=(test%2)==0;
 if(test<2){bool result=p->PeekNextItem(*reinterpret_cast<CParsedItem*>(itemRaw));if(result!=success || itemRaw[0]!=2)return 1;}
 else{EParsedItemType result=p->PeekNextItemType();if(result!=(success?2:0))return 2;}
 if(raw[7]!=(success?0x12345601u:0x76543200u)||raw[8]!=(success?12u:90u)||raw[9]!=(success?34u:80u))return 3;}
 puts("PARSER_PEEK_OK cases=4");return 0;}
