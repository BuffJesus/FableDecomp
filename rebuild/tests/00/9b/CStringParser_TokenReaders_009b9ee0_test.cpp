#include "fable_string_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static CCharStringData*& slot(const CCharString& s){return *reinterpret_cast<CCharStringData**>(const_cast<CCharString*>(&s));}
static const char* text(const CCharString& s){return slot(s)?slot(s)->Data.PData:"";}
static void assign(CCharString& s,const char* data,long length){if(slot(s)){free(slot(s)->Data.PData);free(slot(s));}slot(s)=(CCharStringData*)calloc(1,sizeof(CCharStringData));slot(s)->Data.PData=(char*)malloc(length+1);memcpy(slot(s)->Data.PData,data,length);slot(s)->Data.PData[length]=0;slot(s)->Data.StringLength=length;}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* s,long length):storage_(0){assign(*this,s,length<0?strlen(s):length);}
CCharString::~CCharString(){if(storage_){free(storage_->Data.PData);free(storage_);}}
CCharString::operator const char*()const{return text(*this);}
void CCharString::SetAsSubString(const char* s,long length){assign(*this,s,length);}
void CParsedItem::InitAsInteger(const char* s,unsigned long length){String.SetAsSubString(s,length);Type=INTEGER;}
void CParsedItem::InitAsFloat(const char* s,unsigned long length){String.SetAsSubString(s,length);Type=FLOAT;}
struct View:CStringParser{static bool run(CStringParser* p,CParsedItem& item,bool number){return number?static_cast<View*>(p)->ReadNextNumber(item):static_cast<View*>(p)->ReadNextString(item);}};
int main(){struct Case{char* source;char* value;bool number;int type;}cases[]={
 {"name_12;","name_12",false,2},{"12name ","12name",false,2},{"_id","_id",false,2},{"-x","",false,2},{"","",false,2},
 {"12;","12",true,3},{"-12","-12",true,3},{"1.25","1.25",true,4},{"1.2.3","1.2",true,4},{"1e3","1",true,3},{"12-3","12",true,3},{"-","-",true,3}};
 for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i){unsigned long raw[10]={0};CStringParser* p=reinterpret_cast<CStringParser*>(raw);p->String=cases[i].source;p->InQuotes=true;p->Line=29;CParsedItem item;
 if(!View::run(p,item,cases[i].number)||strcmp(text(item.String),cases[i].value)||item.Type!=cases[i].type||p->StringPos!=strlen(cases[i].value)||!p->InQuotes||p->Line!=29)return 1;}
 puts("PARSER_TOKEN_READERS_OK cases=12");return 0;}
