#include "fable_world.h"
#include <stdio.h>
#include <string.h>
typedef _STL::pair<ETCInterfaceType,CTCBase*> Entry;
typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Map;
// Require the separately compiled original provider rather than an inline copy.
template <> const Entry* Map::Find(const ETCInterfaceType&) const;
static const long keys[5][7]={{0},{17},{(long)0x80000000UL,-1,0,17,17,18,0x7fffffff},{16,18},{0,1,16,48,54,273}};
static const unsigned counts[5]={0,1,7,2,6};
static const long queries[10]={(long)0x80000000UL,-1,0,1,16,17,18,273,274,0x7fffffff};
int main() {
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern) for(unsigned m=0;m<5;++m) {
        unsigned long storage[24],before[24];memset(storage,pattern*71,sizeof(storage));
        Map* mutableMap=reinterpret_cast<Map*>(storage+2);const Map& map=*mutableMap;
        Entry entries[8],entriesBefore[8];memset(entries,pattern*71,sizeof(entries));
        for(unsigned i=0;i<counts[m]+1;++i){entries[i].first=static_cast<ETCInterfaceType>(i<counts[m]?keys[m][i]:0x7fffffff);entries[i].second=reinterpret_cast<CTCBase*>(0x3400040+i*0x20);}
        mutableMap->_M_start=entries;mutableMap->_M_finish=entries+counts[m];mutableMap->_M_end_of_storage._M_data=entries+counts[m]+1;
        memcpy(before,storage,sizeof(storage));memcpy(entriesBefore,entries,sizeof(entries));
        for(unsigned q=0;q<10;++q){ETCInterfaceType key=static_cast<ETCInterfaceType>(queries[q]);unsigned found=0;while(found<counts[m] && keys[m][found]<queries[q])++found;if(found==counts[m] || keys[m][found]!=queries[q])found=counts[m];
            if(map.Find(key)!=entries+found || memcmp(storage,before,sizeof(storage)) || memcmp(entries,entriesBefore,sizeof(entries)))return 1;++scenarios;}
    }
    printf("CONST_COMPONENT_FIND_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
