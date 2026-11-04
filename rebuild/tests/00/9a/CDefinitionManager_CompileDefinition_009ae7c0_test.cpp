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

#include "fable_def_class_base.h"
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CCharString FABLE_FASTCALL operator+(const char* a,const CCharString& b){char s[256];strcpy(s,a);strcat(s,text(b));return CCharString(s);}
void CCharString::SetChar(long index,char ch){storage_->text[index]=ch;}
CDefString::operator CCharString(){return CCharString("INSTANCE");}
void CDefString::SetString(const CCharString& s){TablePos=!strcmp(text(s),"INSTANCE")?42:-1;}
static unsigned test,errors,loads,copies,sets,transfers,checks,subs;static char error[256],body[256],subBody[256],subName[64];static bool defaults,good;static unsigned char transferredFlags;static CDefClassBase* target;static CDefClassBase* parent;static void* vtable[25];
bool CSymbolMap::FindSymbol(const CCharString& name,long& value)const{good &= !strcmp(text(name),"INSTANCE");value=1;return test!=5;}
void CDefinitionManager::LoadDef(unsigned long index) const{++loads;good &= index==1;}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromInstantiationName(const CCharString& name){good &= !strcmp(text(name),"PARENT");if(test==3)return CDefPointer<CDefClassBase>();parent->Lock();return CDefPointer<CDefClassBase>(parent);}
void CDefinitionManager::Error(const CStringParser&,const CCharString& message){++errors;strcpy(error,text(message));}
void CStringParser::Error(const CCharString&){good=false;}
void CDefinitionManager::CompileSubDefinition(CCharString source,const CCharString& name,long index){++subs;strcpy(subBody,text(source));strcpy(subName,text(name));good &= index==1;}
void CPersistContext::CheckUnknownSymbols(){++checks;}
static CDefString* FABLE_FASTCALL getName(void*,void*,CDefString* out){out->TablePos=42;return out;}
static void FABLE_FASTCALL setName(void*,void*,CDefString value){++sets;good &= value.TablePos==42;}
static void FABLE_FASTCALL copyDef(void*,void*,const CDefClassBase* other){++copies;good &= other==parent;((unsigned char*)target)[36]=0xe7;}
static void FABLE_FASTCALL transfer(void*,void*,CPersistContext& context){++transfers;strcpy(body,context.PParser->String);defaults=context.UseDefaults;transferredFlags=((unsigned char*)target)[36];good &= context.PParser->BaseLineOffset==17 && context.PParser->Line==17 && !strcmp(text(*context.PStringToClean),body);}
struct View:CDefinitionManager{static void setup(CDefinitionManager* m,CInstantiatedDefInfo** slots){View* p=static_cast<View*>(m);p->InstantiatedDefs._M_start=slots;p->PSymbolMap.Data=(CSymbolMap*)0x1234;p->PDefinitionSymbolMap.Data=(CSymbolMap*)0x5678;}static void run(CDefinitionManager* m,CStringParser& p,const CCharString& cls,const CCharString& name){static_cast<View*>(m)->CompileDefinition(p,cls,name);}};
int main(){const char* sources[]={"field=1; #end_definition","field=1; #end_definition","specialises PARENT field=1; #end_definition","specialises PARENT field=1; #end_definition","specialises PARENT field=1; #end_definition","field=1; #end_definition","<Part>\na=1;\n<\\Part>\nfield=2; #end_definition"};vtable[10]=(void*)getName;vtable[11]=(void*)setName;vtable[18]=(void*)transfer;vtable[19]=(void*)copyDef;
 for(test=0;test<7;++test){unsigned long managerRaw[52]={0},parserRaw[10]={0},targetRaw[10]={0},parentRaw[10]={0},infoRaw[4]={0};CDefinitionManager* m=(CDefinitionManager*)managerRaw;CStringParser* p=(CStringParser*)parserRaw;target=(CDefClassBase*)targetRaw;parent=(CDefClassBase*)parentRaw;targetRaw[0]=parentRaw[0]=(unsigned long)vtable;targetRaw[1]=parentRaw[1]=2;unsigned char initial=test==1?0xa4:test==2?0xa2:0xa0;((unsigned char*)target)[36]=initial;((unsigned char*)parent)[36]=test==2?1:2;CDefinitionManager::CInstantiatedDefInfo* info=(CDefinitionManager::CInstantiatedDefInfo*)infoRaw;info->PDef.Object=target;CDefinitionManager::CInstantiatedDefInfo* slots[2]={0,info};View::setup(m,slots);p->String=sources[test];p->Line=17;good=true;errors=loads=copies=sets=transfers=checks=subs=0;error[0]=body[0]=subBody[0]=subName[0]=0;CCharString cls("CLASS"),name("INSTANCE");View::run(m,*p,cls,name);
 if(!good||targetRaw[1]!=2||parentRaw[1]!=2)return 1;
 if(test==5){if(errors!=1||strcmp(error,"Unable to find def class called INSTANCE")||loads||transfers)return 2;continue;}
 if(loads!=1||transfers!=1||checks!=1||defaults!=(test!=1&&test!=2)||((unsigned char*)target)[36]!=(test==2?0xe7:initial|5)||copies!=(test==2?1:0)||sets!=(test==2?1:0))return 3;
 if(errors!=(test==3||test==4?1:0))return 4;if(test==3&&strcmp(error,"Def PARENT does not exist."))return 5;if(test==4&&strcmp(error,"Def PARENT is not setup to specialise."))return 6;
 if(test==6){if(subs!=1||strcmp(subName,"Part")||strcmp(subBody,"\na=1;\n")||strcmp(body,"      \n    \n       \nfield=2; "))return 7;}else if(subs||strcmp(body,test>=2&&test<=4?" field=1; ":"field=1; "))return 8;
 if(test==2&&transferredFlags!=0xe6)return 9;}
 puts("DEFINITION_COMPILE_OK cases=7");return 0;}
