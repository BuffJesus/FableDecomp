#include <stdio.h>
#include <string.h>
#include "fable_world.h"
#include "fable_thing_manager.h"

// These tests borrow raw manager/definition storage. The factory's construction,
// allocation, destruction and complete representation are separate boundaries.
int main()
{
    union { unsigned char bytes[0x28]; double alignment; } managers[4];
    union { unsigned char bytes[16]; double alignment; } definitions[4];
    union { unsigned char bytes[sizeof(CThing)]; double alignment; } things[4];
    memset(managers, 0x5a, sizeof(managers));
    memset(definitions, 0x7c, sizeof(definitions));
    memset(things, 0xa5, sizeof(things));
    if (CThing::PeekPCurrentThingManager() != 0) return 1;
    int cases=0;
    for (int repeat=0; repeat<32; ++repeat)
    for (int manager=0; manager<4; ++manager)
    for (int definition=0; definition<4; ++definition)
    for (int thing=0; thing<4; ++thing)
    {
        CThingManager& current=*reinterpret_cast<CThingManager*>(managers[manager].bytes);
        const CGameDefinitionManager* expected=reinterpret_cast<const CGameDefinitionManager*>(definitions[definition].bytes);
        *reinterpret_cast<const CGameDefinitionManager**>(managers[manager].bytes+0x20)=expected;
        unsigned char before[sizeof(managers)];memcpy(before,managers,sizeof(managers));
        CThing::SetCurrentThingManager(current);
        if (CThing::PeekPCurrentThingManager()!=&current) return 2;
        const CThing& receiver=*reinterpret_cast<const CThing*>(things[thing].bytes);
        if (&receiver.PeekDefinitionManager()!=expected) return 3;
        if (memcmp(before,managers,sizeof(managers))) return 4;
        for (unsigned int i=0;i<sizeof(things);++i)
            if (reinterpret_cast<const unsigned char*>(things)[i]!=0xa5) return 5;
        // A rebound manager reference is read freshly without resetting the singleton.
        expected=reinterpret_cast<const CGameDefinitionManager*>(definitions[(definition+1)%4].bytes);
        *reinterpret_cast<const CGameDefinitionManager**>(managers[manager].bytes+0x20)=expected;
        if (&receiver.PeekDefinitionManager()!=expected) return 6;
        ++cases;
    }
    printf("THING_DEFINITION_MANAGER_BEHAVIOR_PASS cases=%d\n",cases);
    return 0;
}
