#include "fable_definition_manager.h"
#include "fable_creature_combat.h"
#include <stdio.h>
#include <string.h>
typedef CDefPointer<const CCombatTypeDef> Pointer;
static void* g_selected;static Pointer* g_output;static void* g_third;static int g_mode,g_calls,g_deletes;
static const CDefinitionManager* g_manager;static const CCharString* g_name;
static void* g_deleted[4];static void* g_observed_output[4];
static void __fastcall DeleteBoundary(void* object,void*){
 g_deleted[g_deletes]=object;g_observed_output[g_deletes]=g_output->Object;++g_deletes;
 if(g_mode)g_output->Object=g_third;
}
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index) const{
 if(this!=g_manager || index!=2)g_calls+=100; ++g_calls;
 if(g_selected)++*reinterpret_cast<unsigned long*>(static_cast<char*>(g_selected)+4);
 return CDefPointer<const CDefClassBase>(static_cast<const CDefClassBase*>(g_selected));
}
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromInstantiationName(const CCharString& name) const{
 if(this!=g_manager || &name!=g_name)g_calls+=100; ++g_calls;
 if(g_selected)++*reinterpret_cast<unsigned long*>(static_cast<char*>(g_selected)+4);
 return CDefPointer<const CDefClassBase>(static_cast<const CDefClassBase*>(g_selected));
}
int main(){
 const unsigned long counts[]={0,1,2,0xffffffffUL};const long indices[]={-1,0,2};int cases=0;
 void* slots[2]={0,reinterpret_cast<void*>(&DeleteBoundary)};
 for(int ca=0;ca<4;++ca)for(int cb=0;cb<4;++cb)for(int cc=0;cc<4;++cc)
 for(int old=0;old<4;++old)for(int selected=0;selected<4;++selected)for(int mode=0;mode<2;++mode)
 for(int named=0;named<2;++named)for(int ix=0;ix<3;++ix){
  union{double align;char bytes[sizeof(CDefPointeeBase)];} objects[3];memset(objects,0x5a,sizeof(objects));
  void* pointers[]={0,objects[0].bytes,objects[1].bytes,objects[2].bytes};unsigned long expected[]={counts[ca],counts[cb],counts[cc]};
  for(int i=0;i<3;++i){*reinterpret_cast<void***>(objects[i].bytes)=slots;*reinterpret_cast<unsigned long*>(objects[i].bytes+4)=expected[i];}
  Pointer output;output.Object=pointers[old];void* query=0;g_name=reinterpret_cast<const CCharString*>(&query);
  union{double align;char bytes[sizeof(CDefinitionManager)];} manager;memset(&manager,0x5a,sizeof(manager));g_manager=reinterpret_cast<const CDefinitionManager*>(&manager);
  g_selected=pointers[selected];g_output=&output;g_third=pointers[3];g_mode=mode;g_calls=g_deletes=0;
  bool called=named || indices[ix]>0;bool success=called && selected!=0;int dest=old,deleted[2],observed[2],deletions=0;
  if(called && selected)++expected[selected-1];
  if(success){
   if(dest!=selected){
    if(dest && --expected[dest-1]==0){deleted[deletions]=dest;observed[deletions++]=dest;}
    dest=selected;++expected[selected-1];
   }
   if(--expected[selected-1]==0){deleted[deletions]=selected;observed[deletions++]=dest;if(mode)dest=3;}
  }
  bool result=named?g_manager->GetDef<CCombatTypeDef>(*g_name,&output):g_manager->GetDef<CCombatTypeDef>(indices[ix],&output);
  if(result!=success || output.Object!=pointers[dest] || g_calls!=int(called) || g_deletes!=deletions)return 1;
  for(int i=0;i<deletions;++i)if(g_deleted[i]!=pointers[deleted[i]] || g_observed_output[i]!=pointers[observed[i]])return 2;
  for(int i=0;i<3;++i){if(*reinterpret_cast<unsigned long*>(objects[i].bytes+4)!=expected[i])return 3;for(unsigned int j=8;j<sizeof(CDefPointeeBase);++j)if(objects[i].bytes[j]!=0x5a)return 4;}
  for(unsigned int j=0;j<sizeof(manager);++j)if(manager.bytes[j]!=0x5a)return 5;
  output.Object=0;++cases;
 }
 printf("COMBAT_DEF_HELPERS_BEHAVIOR_PASS cases=%d\n",cases);return 0;
}
