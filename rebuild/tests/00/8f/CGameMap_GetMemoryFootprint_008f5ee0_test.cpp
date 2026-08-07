#include "rebuild_abi.h"
#include <cstdio>
struct CGameMap { char pad00[0x204]; long m_width; long m_height; long GetMemoryFootprint(); };
long CGameMap::GetMemoryFootprint(){ return (m_height * m_width) * 12; }
int main(){
    CGameMap m; m.m_width=4; m.m_height=5;
    if(m.GetMemoryFootprint()!=240){std::printf("BAD\n");return 1;}
    std::printf("FOOTPRINT_OK\n"); return 0;
}