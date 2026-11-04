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

CCharString FABLE_FASTCALL operator+(const char* a,const CCharString& b){char s[256];strcpy(s,a);strcat(s,text(b));return CCharString(s);}
static int errors,parserErrors;static char message[256];
void CPersistContext::PersistError(const CStringParser&,const CCharString& msg) const{++errors;strcpy(message,text(msg));}
void CStringParser::Error(const CCharString&){++parserErrors;}
bool CSymbolMap::FindSymbol(const CCharString& name,long& val)const{if(!strcmp(text(name),"Known")){val=5;return true;}return false;}
template<> void FABLE_FASTCALL _STL::sort(_STL::pair<CCharString,CSymbolMap::CSymbolInfo>* first,_STL::pair<CCharString,CSymbolMap::CSymbolInfo>* last,CCompareString<CSymbolMap::CSymbolInfo> compare){}
int main(){struct Case{const char* source;long value;unsigned long position;} cases[]={
 {"7;",7,1},{"1|2|4;",7,5},{"10+3-2;",11,6},{"10-3-2;",9,6},{"1<<2+1;",8,6},{"-8>>1;",-4,5},{"1<<33;",2,5},{"2147483647+1;",(-2147483647L-1),12},{"Known+1;",6,7},{"Missing;",0,7},{"2|1+4;",7,5}};
 for(unsigned i=0;i<11;++i){unsigned long parserRaw[10]={0},contextRaw[11]={0},mapRaw[9]={0};CStringParser* p=(CStringParser*)parserRaw;CPersistContext* c=(CPersistContext*)contextRaw;p->String=cases[i].source;c->PSymbolMap=(CSymbolMap*)mapRaw;long value=123;errors=parserErrors=0;message[0]=0;CPersistTraitsSLONG::TransferIn(*c,*p,value);
 if(value!=cases[i].value || p->StringPos!=cases[i].position || parserErrors || errors!=(i==9?1:0) || (i==9 && strcmp(message,"Unable to find symbol Missing"))){printf("case %u value %ld pos %lu errors %d parser %d\n",i,value,p->StringPos,errors,parserErrors);return 1;}}
 puts("PERSIST_SLONG_OK cases=11");return 0;}
