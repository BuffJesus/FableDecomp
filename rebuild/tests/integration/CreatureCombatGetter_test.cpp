#include "fable_creature_combat.h"
#include "fable_thing_manager.h"
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static const CGameDefinitionManager* g_manager;static const CCombatTypeDef* g_lookup;
static int g_index,g_lookup_calls,g_ctor_calls,g_dtor_calls,g_deletes;static const char* g_last_name;
static void* g_deleted[4];static void* g_observed_output[4];static void* g_output;
static void __fastcall DeleteBoundary(void* object,void*){g_deleted[g_deletes]=object;g_observed_output[g_deletes]=g_output;++g_deletes;}
CCharString::CCharString(const char* text,long length){
 static CCharStringData data;data.Data.PData=(char*)text;data.Data.StringLength=length<0?strlen(text):length;
 storage_=&data;++g_ctor_calls;g_last_name=text;
}
CCharString::~CCharString(){storage_=0;++g_dtor_calls;}
template<> bool CDefinitionManager::GetDef<CCombatTypeDef>(long index,CDefPointer<const CCombatTypeDef>* output) const{
 if(this!=g_manager || index!=g_index)g_lookup_calls+=100;
 ++g_lookup_calls;
 if(index<=0 || !g_lookup)return false;
 output->Object=(void*)g_lookup;
 ++*reinterpret_cast<unsigned long*>(static_cast<char*>(output->Object)+4);
 return true;
}
template<> bool CDefinitionManager::GetDef<CCombatTypeDef>(const CCharString& name,CDefPointer<const CCombatTypeDef>* output) const{
 if(this!=g_manager || strcmp(name.GetConstCharPtr(),"COMBAT_TYPE_NULL"))g_lookup_calls+=100;
 ++g_lookup_calls;
 if(!g_lookup)return false;
 output->Object=(void*)g_lookup;
 ++*reinterpret_cast<unsigned long*>(static_cast<char*>(output->Object)+4);
 return true;
}
int main(){
 const unsigned long counts[]={0,1,2,0xffffffffUL};int cases=0;
 void* slots[2]={0,reinterpret_cast<void*>(&DeleteBoundary)};
 for(int has=0;has<2;++has)for(int component=0;component<3;++component)
 for(int variant=1;variant<=3;++variant)for(int ii=0;ii<3;++ii)
 for(int lookup=0;lookup<3;++lookup)for(int ca=0;ca<4;++ca)for(int cb=0;cb<4;++cb){
  int index=ii==0?-2:ii==1?0:3;
  union{double align;char bytes[sizeof(CThingCreatureBase)];} thingStore;
  union{double align;char bytes[sizeof(CThingCreatureDef)];} definitionStore;
  union{double align;char bytes[sizeof(CThingManager)];} managerStore;
  union{double align;char bytes[sizeof(CGameDefinitionManager)];} gameDefinitionStore;
  union{double align;char bytes[sizeof(CTCCombat)];} combatStore;
  union{double align;char bytes[sizeof(CCombatTypeDef)];} objectStore[2];
  memset(&thingStore,0x5a,sizeof(thingStore));memset(&definitionStore,0x5a,sizeof(definitionStore));
  memset(&managerStore,0x5a,sizeof(managerStore));memset(&gameDefinitionStore,0x5a,sizeof(gameDefinitionStore));
  memset(&combatStore,0x5a,sizeof(combatStore));memset(&objectStore,0x5a,sizeof(objectStore));
  void* pointers[]={0,objectStore[0].bytes,objectStore[1].bytes};unsigned long expected[]={counts[ca],counts[cb]};
  for(int i=0;i<2;++i){*reinterpret_cast<void***>(pointers[i+1])=slots;*reinterpret_cast<unsigned long*>(static_cast<char*>(pointers[i+1])+4)=expected[i];}
  CThingCreatureBase* thing=reinterpret_cast<CThingCreatureBase*>(&thingStore);
  CTCCombat* combat=reinterpret_cast<CTCCombat*>(&combatStore);
  *reinterpret_cast<unsigned long*>(thingStore.bytes+0x24)=has?0x10000:0;
  typedef _STL::pair<ETCInterfaceType,CTCBase*> Entry;
  Entry map[3];int keys[3]={17,48,54};int first=variant==1?1:0;int size=variant==3?3:variant==2?2:1;
  for(int i=0;i<size;++i){map[i].first=(ETCInterfaceType)keys[i+first];map[i].second=combat;}
  *reinterpret_cast<Entry**>(thingStore.bytes+0x44)=map;
  *reinterpret_cast<Entry**>(thingStore.bytes+0x48)=map+size;
  *reinterpret_cast<Entry**>(thingStore.bytes+0x4c)=map+3;
  *reinterpret_cast<void**>(combatStore.bytes+0x28)=pointers[component];
  *reinterpret_cast<void**>(thingStore.bytes+0xe0)=definitionStore.bytes;
  *reinterpret_cast<long*>(definitionStore.bytes+0x124)=index;
  *reinterpret_cast<void**>(managerStore.bytes+0x20)=gameDefinitionStore.bytes;
  CThing::SetCurrentThingManager(*reinterpret_cast<CThingManager*>(&managerStore));
  g_manager=reinterpret_cast<const CGameDefinitionManager*>(&gameDefinitionStore);g_lookup=(const CCombatTypeDef*)pointers[lookup];
  g_index=index;g_lookup_calls=g_ctor_calls=g_dtor_calls=g_deletes=0;g_last_name=0;g_output=0;
  if(&static_cast<const CThing&>(*thing).GetTC(TCI_COMBAT)!=combat)return 1;
  char beforeThing[sizeof(thingStore)],beforeDefinition[sizeof(definitionStore)],beforeCombat[sizeof(combatStore)],beforeManager[sizeof(managerStore)],beforeGameDefinition[sizeof(gameDefinitionStore)];
  memcpy(beforeThing,&thingStore,sizeof(thingStore));memcpy(beforeDefinition,&definitionStore,sizeof(definitionStore));
  memcpy(beforeCombat,&combatStore,sizeof(combatStore));memcpy(beforeManager,&managerStore,sizeof(managerStore));memcpy(beforeGameDefinition,&gameDefinitionStore,sizeof(gameDefinitionStore));
  bool fromComponent=has && component;int chosen=fromComponent?component:(index>0 || index==0)?lookup:0;
  if(chosen)++expected[chosen-1];int deletionCount=0;
  if(chosen && (fromComponent?counts[chosen==1?ca:cb]:counts[chosen==1?ca:cb])==0xffffffffUL)deletionCount=fromComponent?2:1;
  CDefPointer<const CCombatTypeDef> result=thing->GetPCombatTypeDef();g_output=result.Object;
  if(result.Object!=pointers[chosen] || g_lookup_calls!=(fromComponent?0:1) || g_ctor_calls!=(!fromComponent&&index==0) || g_dtor_calls!=g_ctor_calls)return 2;
  if(g_deletes!=deletionCount)return 3;
  for(int i=0;i<2;++i){if(*reinterpret_cast<unsigned long*>(static_cast<char*>(pointers[i+1])+4)!=expected[i])return 4;for(unsigned int j=8;j<sizeof(CCombatTypeDef);++j)if(objectStore[i].bytes[j]!=0x5a)return 5;}
  if(memcmp(beforeThing,&thingStore,sizeof(thingStore)) || memcmp(beforeDefinition,&definitionStore,sizeof(definitionStore)) || memcmp(beforeCombat,&combatStore,sizeof(combatStore)) || memcmp(beforeManager,&managerStore,sizeof(managerStore)) || memcmp(beforeGameDefinition,&gameDefinitionStore,sizeof(gameDefinitionStore)))return 6;
  result.Object=0;++cases;
 }
 printf("CREATURE_COMBAT_GETTER_BEHAVIOR_PASS cases=%d\n",cases);return 0;
}
