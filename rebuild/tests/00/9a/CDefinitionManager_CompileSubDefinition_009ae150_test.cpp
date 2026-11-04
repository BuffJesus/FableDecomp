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
static unsigned test,loads,creates,removes,copies,transfers,checks;static unsigned indices[3];static bool good,defaults;static CDefClassBase* parent;static CDefClassBase* original;static CDefClassBase* fresh;static CDefClassBase::CSubDefInfo subInfo;static void* vtable[25];
void CDefinitionManager::LoadDef(unsigned long index)const{indices[loads++]=index;}
CDefPointer<CDefClassBase> CDefinitionManager::InstantiateSubDef(const CCharString& name,unsigned long index){++creates;good &= !strcmp(text(name),"Part")&&index==1;if(test==3)return CDefPointer<CDefClassBase>();fresh->Lock();return CDefPointer<CDefClassBase>(fresh);}
void CPersistContext::CheckUnknownSymbols(){++checks;}
static CDefClassBase::CSubDefInfo* FABLE_FASTCALL getInfo(void*,void*,const CCharString& name){good &= !strcmp(text(name),"Part");return test==1||test==2?&subInfo:0;}
static void FABLE_FASTCALL removeSub(void*,void*,const CCharString& name){++removes;good &= !strcmp(text(name),"Part");}
static void FABLE_FASTCALL copyDef(void* self,void*,const CDefClassBase* other){++copies;good &= self==fresh&&other==original;((unsigned char*)fresh)[36]=((unsigned char*)original)[36];}
static void FABLE_FASTCALL transfer(void* self,void*,CPersistContext& context){++transfers;defaults=context.UseDefaults;good &= self==(test==1?original:fresh)&&!strcmp(context.PParser->String,"field=1;")&&context.PParser->Line==0&&context.PParser->BaseLineOffset==0&&!strcmp(text(*context.PStringToClean),"field=1;");}
struct View:CDefinitionManager{static void setup(CDefinitionManager* m,CInstantiatedDefInfo** slots){View* p=static_cast<View*>(m);p->InstantiatedDefs._M_start=slots;p->PSymbolMap.Data=(CSymbolMap*)0x1234;}static void run(CDefinitionManager* m,CCharString source,const CCharString& name){static_cast<View*>(m)->CompileSubDefinition(source,name,1);}};
int main(){vtable[15]=(void*)getInfo;vtable[13]=(void*)removeSub;vtable[18]=(void*)transfer;vtable[19]=(void*)copyDef;
 for(test=0;test<4;++test){unsigned long managerRaw[52]={0},parentRaw[10]={0},originalRaw[10]={0},freshRaw[10]={0},parentInfoRaw[4]={0},originalInfoRaw[4]={0};parent=(CDefClassBase*)parentRaw;original=(CDefClassBase*)originalRaw;fresh=(CDefClassBase*)freshRaw;parentRaw[0]=originalRaw[0]=freshRaw[0]=(unsigned long)vtable;parentRaw[1]=originalRaw[1]=freshRaw[1]=2;((unsigned char*)parent)[36]=0xa0;((unsigned char*)original)[36]=0xa4;((unsigned char*)fresh)[36]=0xb2;subInfo.DefIndex=2;subInfo.OriginalParentDefIndex=test==1?1:0;CDefinitionManager::CInstantiatedDefInfo* p=(CDefinitionManager::CInstantiatedDefInfo*)parentInfoRaw;CDefinitionManager::CInstantiatedDefInfo* o=(CDefinitionManager::CInstantiatedDefInfo*)originalInfoRaw;p->PDef.Object=parent;o->PDef.Object=original;CDefinitionManager::CInstantiatedDefInfo* slots[3]={0,p,o};CDefinitionManager* m=(CDefinitionManager*)managerRaw;View::setup(m,slots);loads=creates=removes=copies=transfers=checks=0;good=true;CCharString source("field=1;"),name("Part");View::run(m,source,name);
 if(!good||parentRaw[1]!=2||originalRaw[1]!=2||freshRaw[1]!=2||strcmp(text(source),"field=1;"))return 1;
 if(loads!=(test==1||test==2?2:1)||indices[0]!=1||((test==1||test==2)&&indices[1]!=2)||creates!=(test==1?0:1)||removes!=(test==2?1:0)||copies!=(test==2?1:0))return 2;
 if(test==3){if(transfers||checks)return 3;}else if(transfers!=1||checks!=1||defaults!=(test==0)||((unsigned char*)(test==1?original:fresh))[36]!=(test==0?0xb7:0xa5))return 4;}
 puts("DEFINITION_SUBCOMPILE_OK cases=4");return 0;}
