#include "fable_definition_manager.h"
#include "fable_string_parser.h"
#include "fable_persist_context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static CCharStringData*& slot(const CCharString& s){return *reinterpret_cast<CCharStringData**>(const_cast<CCharString*>(&s));}
static const char* text(const CCharString& s){return slot(s)?slot(s)->Data.PData:"";}
static CCharStringData* make(const char* s,long length){if(length<0)length=strlen(s);CCharStringData* d=(CCharStringData*)calloc(1,sizeof(CCharStringData));d->Data.PData=(char*)malloc(length+1);memcpy(d->Data.PData,s,length);d->Data.PData[length]=0;d->Data.StringLength=length;return d;}
static void release(CCharStringData* d){if(d){free(d->Data.PData);free(d);}}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* s,long length):storage_(make(s,length)){}
CCharString::CCharString(const CCharString& s):storage_(make(text(s),-1)){}
CCharString::~CCharString(){release(storage_);}
CCharString::operator const char*()const{return text(*this);}
CCharString& CCharString::operator=(const CCharString& s){CCharStringData* d=make(text(s),-1);release(storage_);storage_=d;return *this;}
const CCharString& CCharString::operator=(const char* s){CCharStringData* d=make(s,-1);release(storage_);storage_=d;return *this;}
void CCharString::SetAsSubString(const char* s,long length){CCharStringData* d=make(s,length);release(storage_);storage_=d;}
template<>bool CBasicString<char>::operator==(const char* s)const{return !strcmp(PData,s);}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const CCharString& b){char s[256];strcpy(s,text(a));strcat(s,text(b));return CCharString(s);}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const char* b){char s[256];strcpy(s,text(a));strcat(s,b);return CCharString(s);}
static int count,errors,parserErrors;static char groups[8][64],names[8][64],message[256];static long values[8];
void CSymbolMap::AddSymbolDirty(const CCharString& group,const CCharString& name,long value){strcpy(groups[count],text(group));strcpy(names[count],text(name));values[count++]=value;}
void CDefinitionManager::Error(const CStringParser&,const CCharString& msg){++errors;strcpy(message,text(msg));}
void CStringParser::Error(const CCharString&){++parserErrors;}
void FABLE_FASTCALL CPersistTraitsSLONG::TransferIn(CPersistContext&,CStringParser& parser,long& value){CParsedItem item;value=parser.ReadNextItem(item)?item.GetInteger():0;}
struct View:CDefinitionManager{static void run(CDefinitionManager* m,CStringParser& p){static_cast<View*>(m)->ParseEnumForSymbols(p);}static void setup(CDefinitionManager* m){static_cast<View*>(m)->PSymbolMap.Data=reinterpret_cast<CSymbolMap*>(0x1234);}};
int main(){struct Case{char* source;int count,errors;const char* message;}cases[]={
 {"E { A, B=7, C, };",3,0,""},{"{ A, };",1,0,""},{"E { 1 };",0,1,"Expected string in enum"},{"E { }: ",0,1,"Unexpected symbol in enum."},{"E { A: };",0,1,"Unknown symbol after 'A'."},{"E { A,",1,0,""},{"E { A };",0,0,""},{"E { A=7 };",1,0,""}};
 for(unsigned i=0;i<8;++i){unsigned long managerRaw[52]={0},parserRaw[10]={0};CDefinitionManager* m=reinterpret_cast<CDefinitionManager*>(managerRaw);CStringParser* p=reinterpret_cast<CStringParser*>(parserRaw);View::setup(m);p->String=cases[i].source;count=errors=parserErrors=0;message[0]=0;View::run(m,*p);
 if(count!=cases[i].count||errors!=cases[i].errors||parserErrors||strcmp(message,cases[i].message)){printf("case %u count %d errors %d parserErrors %d message %s\n",i,count,errors,parserErrors,message);return 1;}
 if(i==0&&(strcmp(groups[0],"E")||strcmp(names[0],"A")||values[0]!=0||strcmp(names[1],"B")||values[1]!=7||strcmp(names[2],"C")||values[2]!=8))return 2;
 if(i==1&&(strcmp(groups[0],"NULL")||values[0]!=0))return 3;
 if(i==7&&values[0]!=7)return 4;}
 puts("DEFINITION_ENUM_PARSER_OK cases=8");return 0;}
