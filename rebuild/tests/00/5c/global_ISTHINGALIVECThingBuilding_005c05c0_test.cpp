#include "rebuild_abi.h"
#include <cstdio>
struct CThingBuilding { char pad[0x91]; unsigned char m_flags; };
struct Wrap { CThingBuilding *m_p; };
extern "C" char FABLE_FASTCALL FableIsAlive_5c05c0(Wrap *w){
    CThingBuilding *p=w->m_p; if(p && !(p->m_flags&1)) return 1; return 0; }
int main(){ CThingBuilding b; b.m_flags=0; Wrap w; w.m_p=&b;
 if(FableIsAlive_5c05c0(&w)!=1){std::printf("B1\n");return 1;}
 b.m_flags=1; if(FableIsAlive_5c05c0(&w)!=0){std::printf("B2\n");return 1;}
 w.m_p=0; if(FableIsAlive_5c05c0(&w)!=0){std::printf("B3\n");return 1;}
 std::printf("IA_5c05c0_OK\n"); return 0; }