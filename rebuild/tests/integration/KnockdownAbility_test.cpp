#include "fable_thing_components.h"
#include "fable_creature_combat.h"
#include <stdio.h>
#include <string.h>
static unsigned long combatStorage[80],creatureStorage[2][80],definitionStorage[2][80];
static CTCCombat* combat;
static const CThingCreatureBase* expectedCreature;
static unsigned selected,effect,getCalls,deleteCalls;
static unsigned char* bytes(void* p){return reinterpret_cast<unsigned char*>(p);}
static CCombatTypeDef* definition(unsigned i){return reinterpret_cast<CCombatTypeDef*>(definitionStorage[i]+2);}
static void __fastcall DeleteBoundary(const CIVCountedPointeeBase* self,void*)
{
    if(self!=definition(selected) || self->IVRefCount!=0)deleteCalls=100;
    ++deleteCalls;
    if(effect==1)bytes(definition(selected))[0x9c]=bytes(definition(selected))[0x9c]?0:255;
    if(effect==2){*reinterpret_cast<const CThingCreatureBase**>(bytes(combat)+0x24)=reinterpret_cast<const CThingCreatureBase*>(creatureStorage[1]+2);bytes(definition(1-selected))[0x9c]=77;}
}
CDefPointer<const CCombatTypeDef> CThingCreatureBase::GetPCombatTypeDef() const
{
    if(this!=expectedCreature)getCalls=100;
    ++getCalls;
    if(effect==3)*reinterpret_cast<const CThingCreatureBase**>(bytes(combat)+0x24)=reinterpret_cast<const CThingCreatureBase*>(creatureStorage[1]+2);
    return CDefPointer<const CCombatTypeDef>(definition(selected));
}
int main()
{
    unsigned long vtable[2]={0,reinterpret_cast<unsigned long>(&DeleteBoundary)};
    const unsigned long refs[4]={1,2,0xffffffffUL,0};unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern)for(unsigned initial=0;initial<256;++initial)for(unsigned ownership=0;ownership<4;++ownership)for(selected=0;selected<2;++selected)for(effect=0;effect<4;++effect)for(unsigned entry=0;entry<2;++entry){
        memset(combatStorage,pattern*71,sizeof(combatStorage));memset(creatureStorage,pattern*71,sizeof(creatureStorage));memset(definitionStorage,pattern*71,sizeof(definitionStorage));combat=reinterpret_cast<CTCCombat*>(combatStorage+2);expectedCreature=reinterpret_cast<const CThingCreatureBase*>(creatureStorage[0]+2);
        *reinterpret_cast<const CThingCreatureBase**>(bytes(combat)+0x24)=expectedCreature;
        for(unsigned i=0;i<2;++i){*reinterpret_cast<unsigned long**>(definition(i))=vtable;definition(i)->IVRefCount=refs[ownership];bytes(definition(i))[0x9c]=static_cast<unsigned char>(i==selected?initial:93);}
        unsigned long expectedCombat[80],expectedCreatures[2][80],expectedDefs[2][80];memcpy(expectedCombat,combatStorage,sizeof(expectedCombat));memcpy(expectedCreatures,creatureStorage,sizeof(expectedCreatures));memcpy(expectedDefs,definitionStorage,sizeof(expectedDefs));
        unsigned long remaining=refs[ownership]-1;*reinterpret_cast<unsigned long*>(bytes(expectedDefs[selected]+2)+4)=remaining;unsigned expectedFlag=initial;
        if(remaining==0 && effect==1){expectedFlag=initial?0:255;bytes(expectedDefs[selected]+2)[0x9c]=static_cast<unsigned char>(expectedFlag);}
        if((remaining==0 && effect==2)||effect==3)*reinterpret_cast<const CThingCreatureBase**>(bytes(expectedCombat+2)+0x24)=reinterpret_cast<const CThingCreatureBase*>(creatureStorage[1]+2);
        if(remaining==0 && effect==2)bytes(expectedDefs[1-selected]+2)[0x9c]=77;
        getCalls=deleteCalls=0;
        if(entry==0){bool result=combat->IsAbleToBeKnockedDown();unsigned char raw;memcpy(&raw,&result,1);if(raw!=(expectedFlag!=0))return 1;}
        else if(&expectedCreature->GetCombatTypeDef()!=definition(selected))return 2;
        if(getCalls!=1 || deleteCalls!=(remaining==0) || memcmp(expectedCombat,combatStorage,sizeof(expectedCombat)) || memcmp(expectedCreatures,creatureStorage,sizeof(expectedCreatures)) || memcmp(expectedDefs,definitionStorage,sizeof(expectedDefs)))return 3;
        ++scenarios;
    }
    printf("KNOCKDOWN_ABILITY_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
