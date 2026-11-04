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
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
static int count,errors;static char classes[8][64],names[8][64];
void CStringParser::Error(const CCharString&){++errors;}
void CDefinitionManager::CompileDefinition(CStringParser&,const CCharString& cls,const CCharString& name){strcpy(classes[count],text(cls));strcpy(names[count++],text(name));}
struct View:CDefinitionManager{static void run(CDefinitionManager* p,const char* source,const CCharString& filter){static_cast<View*>(p)->CompileString(source,filter);}};
int main(){const char* source="#definition ENGINE E; #definition_template CREATURE BASE; #definition CREATURE C; #definition_template ENGINE T;";
 const char* filters[]={"","CREATURE","creature","CRE","ENGINE"};int expected[]={4,2,0,0,2};
 for(unsigned i=0;i<5;++i){unsigned long raw[52]={0};CDefinitionManager* m=reinterpret_cast<CDefinitionManager*>(raw);CCharString filter(filters[i]);count=errors=0;View::run(m,source,filter);if(count!=expected[i]||errors)return 1;
 if(i==0&&(strcmp(names[0],"BASE")||strcmp(names[1],"T")||strcmp(names[2],"E")||strcmp(names[3],"C")))return 2;
 if(i==1&&(strcmp(classes[0],"CREATURE")||strcmp(names[0],"BASE")||strcmp(names[1],"C")))return 3;
 if(i==4&&(strcmp(names[0],"T")||strcmp(names[1],"E")))return 4;}
 unsigned long raw[52]={0};CCharString filter("");count=errors=0;View::run(reinterpret_cast<CDefinitionManager*>(raw),"",filter);if(count||errors)return 5;
 puts("DEFINITION_COMPILE_STRING_OK cases=6");return 0;}
