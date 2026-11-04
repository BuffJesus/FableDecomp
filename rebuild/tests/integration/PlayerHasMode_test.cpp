#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>
struct CPlayerLookupTestAccess{static void Setup(CPlayer& p,_STL::_List_node<EPlayerMode>* sentinel){p.PlayerModes._M_node=sentinel;}};
static const long keys[6][7]={{0},{1},{19,1,40},{1,1,1},{(long)0x80000000UL,-1,0,1,19,40,0x7fffffff},{40,19,2,43,17,16,1}};
static const unsigned counts[6]={0,1,3,3,7,7};
static const long queries[12]={(long)0x80000000UL,-1,0,1,2,16,17,19,40,43,273,0x7fffffff};
int main(){unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern)for(unsigned list=0;list<6;++list)for(unsigned sentinel=0;sentinel<12;++sentinel){unsigned long storage[160],before[160];memset(storage,pattern*71,sizeof(storage));CPlayer* player=reinterpret_cast<CPlayer*>(storage+2);_STL::_List_node<EPlayerMode> nodes[8],nb[8];memset(nodes,pattern*71,sizeof(nodes));unsigned n=counts[list];nodes[0]._M_next=n?nodes+1:nodes;nodes[0]._M_prev=nodes+n;nodes[0]._M_data=static_cast<EPlayerMode>(queries[sentinel]);for(unsigned i=1;i<=n;++i){nodes[i]._M_next=i<n?nodes+i+1:nodes;nodes[i]._M_prev=nodes+i-1;nodes[i]._M_data=static_cast<EPlayerMode>(keys[list][i-1]);}CPlayerLookupTestAccess::Setup(*player,nodes);memcpy(before,storage,sizeof(storage));memcpy(nb,nodes,sizeof(nodes));
        for(unsigned q=0;q<12;++q){bool found=false;for(unsigned i=0;i<n;++i)if(keys[list][i]==queries[q])found=true;if(player->HasMode(static_cast<EPlayerMode>(queries[q]))!=found || memcmp(before,storage,sizeof(storage)) || memcmp(nb,nodes,sizeof(nodes)))return 1;++scenarios;}
    }
    printf("PLAYER_HAS_MODE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
