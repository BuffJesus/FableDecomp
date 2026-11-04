#include "fable_world.h"
#include "fable_player_manager.h"
#include "fable_game_event.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
static void* expectedDelete;
static unsigned deletes,errors;
void* operator new(size_t n) { return malloc(n); }
void operator delete(void* p) {if(p!=expectedDelete) ++errors;++deletes;free(p);}
struct CBaseIntelligentPointerTestAccess {
    static void Set(CBaseIntelligentPointer* p,CBaseObjectPointer* data) {p->PData=data;}
    static CBaseObjectPointer* Data(CBaseIntelligentPointer* p) {return p->PData;}
};
struct CPlayerControlTestAccess {
    static CBaseIntelligentPointer* Pointer(CPlayer* p) {return &p->ControlledCreature;}
};
struct CPlayerLookupTestAccess {
    static void Set(CPlayerManager* m,CPlayer** entries) {m->Players._M_start=entries;m->Players._M_finish=entries+1;m->Players._M_end_of_storage._M_data=entries+1;m->PlayerNeutral=0;}
    static void Number(CPlayer* p,long n) {p->Number=n;}
};
int main() {
    const unsigned long counts[]={0,1,2,0xffffffff};unsigned scenarios=0;
    for(unsigned entry=0;entry<3;++entry)
    for(unsigned state=0;state<3;++state)
    for(unsigned count=0;count<4;++count)
    for(unsigned value=0;value<256;++value) {
        unsigned long storage[144],creature[48],manager[16],world[104];
        memset(storage,0xa9,sizeof(storage));memset(creature,0xb7,sizeof(creature));memset(manager,0xc5,sizeof(manager));memset(world,0xd3,sizeof(world));
        CPlayer* p=reinterpret_cast<CPlayer*>(&storage[2]);CThingPlayerCreature* target=reinterpret_cast<CThingPlayerCreature*>(&creature[2]);
        CPlayerManager* m=reinterpret_cast<CPlayerManager*>(&manager[2]);CWorld* w=reinterpret_cast<CWorld*>(&world[2]);w->PlayerManager=m;
        CGameEvent event(0);event.Player=static_cast<signed char>(value);CPlayer* entries[]={p};CPlayerLookupTestAccess::Set(m,entries);CPlayerLookupTestAccess::Number(p,event.Player);
        CBaseObjectPointer* data=static_cast<CBaseObjectPointer*>(malloc(8));data->object=state==2?target:0;data->refcount=counts[count];
        CBaseIntelligentPointer* pointer=CPlayerControlTestAccess::Pointer(p);
        if(reinterpret_cast<char*>(pointer)-reinterpret_cast<char*>(p)!=0x2c || reinterpret_cast<CBaseObject*>(target)!=reinterpret_cast<CBaseObject*>(creature+2)) return 1;
        CBaseIntelligentPointerTestAccess::Set(pointer,state?data:0);
        unsigned long before[144];memcpy(before,storage,sizeof(storage));if(state==1) before[2+0x30/4]=0;
        expectedDelete=state==1&&counts[count]==1?data:0;deletes=errors=0;
        const CThingPlayerCreature* got=0;
        if(entry==0) got=p->GetPControlledCreature();
        else if(entry==1) got=p->PeekPControlledCreature();
        else w->EAOpenPCMSNChatMenu(&event);
        ++scenarios;
        if((entry<2&&got!=(state==2?target:0)) || deletes!=unsigned(expectedDelete!=0) || errors || memcmp(before,storage,sizeof(storage))) return 2;
        if(!expectedDelete) {if(data->refcount!=counts[count]-unsigned(state==1)) return 3;free(data);}
    }
    printf("PLAYER_CREATURE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
