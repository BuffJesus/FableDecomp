#include "fable_world.h"
#include <stdio.h>
#include <string.h>
typedef _STL::pair<ETCInterfaceType,CTCBase*> Entry;
typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Map;
struct CThingComponentTestAccess {static void Setup(CThing& thing,Entry* entries,unsigned count,bool has) {thing.UsingComponents.reset();thing.UsingComponents.set(TCI_INVENTORY,has);thing.Components._M_start=entries;thing.Components._M_finish=entries+count;thing.Components._M_end_of_storage._M_data=entries+count+1;}static const Map& Get(const CThing& thing) {return thing.Components;}};
static const long keys[5][7]={{0},{17},{(long)0x80000000UL,-1,0,17,17,18,0x7fffffff},{16,18},{0,1,16,48,54,273}};static const unsigned counts[5]={0,1,7,2,6};static const long queries[10]={(long)0x80000000UL,-1,0,1,16,17,18,273,274,0x7fffffff};
int main() {
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern) for(unsigned map=0;map<5;++map) {
        unsigned long storage[128],before[128];memset(storage,pattern*71,sizeof(storage));CThing* mutableThing=reinterpret_cast<CThing*>(storage+2);const CThing& thing=*mutableThing;Entry entries[8],entriesBefore[8];for(unsigned i=0;i<counts[map]+1;++i) {entries[i].first=static_cast<ETCInterfaceType>(i<counts[map]?keys[map][i]:0x7fffffff);entries[i].second=reinterpret_cast<CTCBase*>(0x3400040+i*0x20);}
        for(unsigned query=0;query<10;++query) {
            ETCInterfaceType key=static_cast<ETCInterfaceType>(queries[query]);CThingComponentTestAccess::Setup(*mutableThing,entries,counts[map],false);memcpy(before,storage,sizeof(storage));memcpy(entriesBefore,entries,sizeof(entries));unsigned index=0;while(index<counts[map] && keys[map][index]<queries[query]) ++index;
            if(CThingComponentTestAccess::Get(thing).LowerBound(key)!=entries+index || memcmp(storage,before,sizeof(storage)) || memcmp(entries,entriesBefore,sizeof(entries))) return 1;++scenarios;
            unsigned found=index<counts[map] && keys[map][index]==queries[query]?index:counts[map];
            for(unsigned present=0;present<2;++present) {
                CThingComponentTestAccess::Setup(*mutableThing,entries,counts[map],present!=0);memcpy(before,storage,sizeof(storage));if(&thing.GetTC(key)!=entries[found].second || memcmp(storage,before,sizeof(storage)) || memcmp(entries,entriesBefore,sizeof(entries))) return 2;++scenarios;
            }
        }
    }
    printf("CONST_COMPONENT_LOOKUP_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
