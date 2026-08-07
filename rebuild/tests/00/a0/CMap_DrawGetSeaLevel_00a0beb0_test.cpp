#include "rebuild_abi.h"
#include <cstdio>
struct CMap { char pad00[0x2c]; float m_seaLevel; float DrawGetSeaLevel(); };
float CMap::DrawGetSeaLevel(){ return m_seaLevel; }
int main(){
    CMap m; m.m_seaLevel=3.5f;
    if(m.DrawGetSeaLevel()!=3.5f){std::printf("BAD\n");return 1;}
    std::printf("SEALVL_OK\n"); return 0;
}