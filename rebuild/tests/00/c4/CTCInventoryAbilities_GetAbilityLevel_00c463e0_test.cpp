#include "rebuild_abi.h"
#include <cstdio>
struct CAbilityRec { long level; long b; long c; };
struct CTCInventoryAbilities { char pad00[8]; CAbilityRec *m_abilities; long GetAbilityLevel(long index); };
long CTCInventoryAbilities::GetAbilityLevel(long index){ return m_abilities[index].level; }
int main(){
    CAbilityRec r[3]; r[0].level=10; r[2].level=30;
    CTCInventoryAbilities a; a.m_abilities=r;
    if(a.GetAbilityLevel(0)!=10||a.GetAbilityLevel(2)!=30){std::printf("BAD\n");return 1;}
    std::printf("ABILITYLVL_OK\n"); return 0;
}