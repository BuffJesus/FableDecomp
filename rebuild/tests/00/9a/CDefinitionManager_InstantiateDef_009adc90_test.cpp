#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char*& slot(const CCharString& s){return *reinterpret_cast<char**>(const_cast<CCharString*>(&s));}
static const char* text(const CCharString& s){return slot(s)?slot(s):"";}
CCharString::CCharString(const char* s,long):storage_(0){slot(*this)=(char*)malloc(strlen(s)+1);strcpy(slot(*this),s);}
CCharString::~CCharString(){free(slot(*this));}
static long expectedGlobal,expectedClass;static bool parent,good,unknown;static int allocations,deletes,sets,interns,pubs,grows;static void* vtable[25];static CDefinitionManager::CDefClassInfo* info;
static bool FABLE_FASTCALL isParent(void*,void*){return parent;}
static void FABLE_FASTCALL setName(void*,void*,CDefString name){++sets;good &= name.TablePos==200;}
static void FABLE_FASTCALL deleteDef(void* object,void*){++deletes;free(object);}
static CDefClassBase* FABLE_FASTCALL allocate(){++allocations;unsigned char* p=(unsigned char*)calloc(1,sizeof(CDefClassBase));*(void***)p=vtable;*(long*)(p+4)=1;p[0x24]=0xa5;return reinterpret_cast<CDefClassBase*>(p);}
CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(const CCharString&){return unknown?0:info;}
void CDefString::SetString(const CCharString& value){++interns;TablePos=!strcmp(text(value),"CANON")?100:!strcmp(text(value),"INSTANCE")?200:-1;good &= TablePos!=-1;}
void CSymbolMap::AddSymbolDirty(const CCharString& name,long value){++pubs;good &= this==reinterpret_cast<CSymbolMap*>(0x1234)&&!strcmp(text(name),"INSTANCE")&&value==expectedGlobal;}
void CSymbolMap::AddSymbolDirty(const CCharString& group,const CCharString& name,long value){++pubs;good &= this==reinterpret_cast<CSymbolMap*>(0x5678)&&!strcmp(text(group),"CANON")&&!strcmp(text(name),"INSTANCE")&&value==expectedGlobal;}
struct View:CDefinitionManager{
 static void setup(CDefinitionManager* m,bool overflow,unsigned initial=3){View* p=static_cast<View*>(m);unsigned capacity=overflow?initial:8;p->InstantiatedDefs._M_start=capacity?(CInstantiatedDefInfo**)calloc(capacity,sizeof(CInstantiatedDefInfo*)):0;p->InstantiatedDefs._M_finish=p->InstantiatedDefs._M_start+initial;p->InstantiatedDefs._M_end_of_storage._M_data=p->InstantiatedDefs._M_start+capacity;p->PSymbolMap.Data=reinterpret_cast<CSymbolMap*>(0x1234);p->PDefinitionSymbolMap.Data=reinterpret_cast<CSymbolMap*>(0x5678);}
 static bool run(CDefinitionManager* p,const CCharString& cls,const CCharString& name,bool flag){return static_cast<View*>(p)->InstantiateDef(cls,name,flag);}
 static CInstantiatedDefInfo* result(CDefinitionManager* p){return static_cast<View*>(p)->InstantiatedDefs[expectedGlobal];}
 static unsigned size(CDefinitionManager* p){return static_cast<View*>(p)->InstantiatedDefs.size();}
 static void cleanup(CDefinitionManager* p){free(static_cast<View*>(p)->InstantiatedDefs._M_start);}
};
int main(){vtable[1]=reinterpret_cast<void*>(&deleteDef);vtable[11]=reinterpret_cast<void*>(&setName);vtable[16]=reinterpret_cast<void*>(&isParent);
 for(unsigned test=0;test<5;++test){unsigned long managerRaw[52]={0},infoRaw[5]={0};CDefinitionManager* m=reinterpret_cast<CDefinitionManager*>(managerRaw);info=reinterpret_cast<CDefinitionManager::CDefClassInfo*>(infoRaw);slot(info->Name)="CANON";info->AllocFunc=allocate;expectedGlobal=test==4?0:3;expectedClass=test==4?0:2;unsigned capacity=test>=3?expectedClass:8;info->GlobalIndices._M_start=capacity?(unsigned long*)calloc(capacity,4):0;info->GlobalIndices._M_finish=info->GlobalIndices._M_start+expectedClass;info->GlobalIndices._M_end_of_storage._M_data=info->GlobalIndices._M_start+capacity;View::setup(m,test>=3,expectedGlobal);unknown=test==0;parent=test>=2;good=true;allocations=deletes=sets=interns=pubs=grows=0;CCharString cls("requested"),name("INSTANCE");bool flag=test!=2;bool result=View::run(m,cls,name,flag);
 if(!good||result==unknown)return 1;
 if(unknown){if(allocations||pubs||View::size(m)!=3)return 2;}
 else{CDefinitionManager::CInstantiatedDefInfo* item=View::result(m);CDefClassBase* def=item->PDef.Get();if(allocations!=1||pubs!=2||sets!=(parent?1:0)||interns!=(parent?3:2)||View::size(m)!=expectedGlobal+1||info->GlobalIndices.size()!=expectedClass+1||info->GlobalIndices[expectedClass]!=expectedGlobal||item->ClassName.TablePos!=100||item->InstantiationName.TablePos!=200||item->ClassIndex!=expectedClass||def->PDefManager!=m||def->GlobalIndex!=expectedGlobal||def->IVRefCount!=1||((unsigned char*)def)[0x24]!=(flag?0xa7:0xa5))return 3;delete item;if(deletes!=1)return 4;}
 View::cleanup(m);free(info->GlobalIndices._M_start);}
 puts("DEFINITION_INSTANTIATE_OK cases=5");return 0;}
