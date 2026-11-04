#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char*& slot(const CCharString& s){return *reinterpret_cast<char**>(const_cast<CCharString*>(&s));}
static const char* text(const CCharString& s){return slot(s)?slot(s):"";}
CCharString::CCharString(const char* s,long):storage_(0){slot(*this)=(char*)malloc(strlen(s)+1);strcpy(slot(*this),s);}
CCharString::~CCharString(){free(slot(*this));}
static bool good,unknown;static int allocations,deletes,loads,adds,interns;static unsigned expectedClass;static void* vtable[25];static CDefinitionManager::CDefClassInfo* info;static CDefClassBase* parent;
static void FABLE_FASTCALL deleteDef(void* object,void*){++deletes;free(object);}
static void FABLE_FASTCALL addSub(void* self,void*,const CCharString& name,long index,long parentIndex){++adds;good &= self==parent&&!strcmp(text(name),"CANON")&&index==3&&parentIndex==1;}
static CDefClassBase* FABLE_FASTCALL allocate(){++allocations;unsigned char* p=(unsigned char*)calloc(1,sizeof(CDefClassBase));*(void***)p=vtable;*(long*)(p+4)=1;p[36]=0xa7;return (CDefClassBase*)p;}
CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(const CCharString& name){good &= !strcmp(text(name),"requested");return unknown?0:info;}
void CDefinitionManager::LoadDef(unsigned long index)const{++loads;good &= index==1;}
void CDefString::SetString(const CCharString& value){++interns;good &= !strcmp(text(value),"CANON");TablePos=100;}
struct View:CDefinitionManager{static void setup(CDefinitionManager* m,unsigned cap,CInstantiatedDefInfo* parentInfo){View* p=static_cast<View*>(m);p->InstantiatedDefs._M_start=(CInstantiatedDefInfo**)calloc(cap,4);p->InstantiatedDefs._M_finish=p->InstantiatedDefs._M_start+3;p->InstantiatedDefs._M_end_of_storage._M_data=p->InstantiatedDefs._M_start+cap;p->InstantiatedDefs[1]=parentInfo;}static CDefPointer<CDefClassBase> run(CDefinitionManager* m,const CCharString& name){return static_cast<View*>(m)->InstantiateSubDef(name,1);}static CInstantiatedDefInfo* result(CDefinitionManager* m){return static_cast<View*>(m)->InstantiatedDefs[3];}static unsigned size(CDefinitionManager* m){return static_cast<View*>(m)->InstantiatedDefs.size();}static void cleanup(CDefinitionManager* m){free(static_cast<View*>(m)->InstantiatedDefs._M_start);}};
int main(){vtable[1]=(void*)deleteDef;vtable[12]=(void*)addSub;
 for(unsigned test=0;test<4;++test){unsigned long managerRaw[52]={0},infoRaw[5]={0},parentRaw[10]={0},parentInfoRaw[4]={0};CDefinitionManager* m=(CDefinitionManager*)managerRaw;info=(CDefinitionManager::CDefClassInfo*)infoRaw;slot(info->Name)="CANON";info->AllocFunc=allocate;expectedClass=test==3?0:2;unsigned cap=test>=2?expectedClass:8;info->GlobalIndices._M_start=cap?(unsigned long*)calloc(cap,4):0;info->GlobalIndices._M_finish=info->GlobalIndices._M_start+expectedClass;info->GlobalIndices._M_end_of_storage._M_data=info->GlobalIndices._M_start+cap;parent=(CDefClassBase*)parentRaw;parentRaw[0]=(unsigned long)vtable;parentRaw[1]=2;CDefinitionManager::CInstantiatedDefInfo* parentInfo=(CDefinitionManager::CInstantiatedDefInfo*)parentInfoRaw;parentInfo->PDef.Object=parent;View::setup(m,test==2?3:8,parentInfo);allocations=deletes=loads=adds=interns=0;good=true;unknown=test==0;CCharString name("requested");CDefPointer<CDefClassBase> result=View::run(m,name);
 if(!good||loads!=1||parentRaw[1]!=2)return 1;
 if(unknown){if(result.Get()||allocations||adds||interns||View::size(m)!=3)return 2;}else{CDefinitionManager::CInstantiatedDefInfo* item=View::result(m);CDefClassBase* child=result.Get();if(allocations!=1||adds!=1||interns!=1||View::size(m)!=4||item->ClassName.TablePos!=100||item->InstantiationName.TablePos!=-1||item->ClassIndex!=expectedClass||item->PDef.Get()!=child||child->IVRefCount!=2||child->PDefManager!=m||child->GlobalIndex!=3||((unsigned char*)child)[36]!=0xa5||info->GlobalIndices.size()!=expectedClass+1||info->GlobalIndices[expectedClass]!=3)return 3;delete item;result.Release();if(deletes!=1)return 4;}
 View::cleanup(m);free(info->GlobalIndices._M_start);}
 puts("DEFINITION_INSTANTIATE_SUB_OK cases=4");return 0;}
