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
const CCharString& CCharString::operator+=(const char* suffix){char result[256];strcpy(result,text(*this));strcat(result,suffix);*this=result;return *this;}
int main(){struct Case{const char* source;bool initial,expected;unsigned long position;bool error;} cases[]={
 {"TRUE;",false,true,4,false},{"true;",false,true,4,false},{"BTRUE;",false,true,5,false},{"bTrUe;",false,true,5,false},
 {"FALSE;",true,false,5,false},{"false;",true,false,5,false},{"BFALSE;",true,false,6,false},{"bFaLsE;",true,false,6,false},
 {"Unknown;",false,false,7,true},{"Unknown;",true,true,7,true},{"1;",false,false,1,true},{"0;",true,true,1,true}};
 for(unsigned i=0;i<12;++i){unsigned long parserRaw[10]={0},contextRaw[11]={0};CStringParser* p=(CStringParser*)parserRaw;CPersistContext* c=(CPersistContext*)contextRaw;p->String=cases[i].source;bool value=cases[i].initial;errors=parserErrors=0;message[0]=0;CPersistContext_TransferableComponent<bool> adapter(value);adapter.TransferIn(*c,*p);
 if(value!=cases[i].expected || p->StringPos!=cases[i].position || parserErrors!=(i>=10?1:0) || errors!=cases[i].error){printf("case %u value %d pos %lu errors %d parser %d\n",i,value,p->StringPos,errors,parserErrors);return 1;}
 if(cases[i].error && strncmp(message,"Unknown boolean value entry ",28))return 2;}
 puts("PERSIST_BOOL_INPUT_OK cases=12");return 0;}
