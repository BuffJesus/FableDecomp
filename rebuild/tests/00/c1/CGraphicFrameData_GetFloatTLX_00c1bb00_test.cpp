#include "rebuild_abi.h"
#include <cstdio>
const float kTexScale_c1bb00 = 0.00390625f;
struct CGraphicFrameData { char pad00[0xe]; unsigned short m_coord; float GetFloatTLX(); };
float CGraphicFrameData::GetFloatTLX(){ return (float)m_coord * kTexScale_c1bb00; }
int main(){
    CGraphicFrameData g; g.m_coord=512;
    if(g.GetFloatTLX()!=2.0f){std::printf("BAD\n");return 1;}
    std::printf("TLXBB00_OK\n"); return 0;
}