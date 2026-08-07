#include "rebuild_abi.h"
#include <cstdio>
const float kTexScale_c1b9e0 = 0.00390625f;
struct CGraphicFrameData { char pad00[8]; unsigned short m_tlx; float GetFloatTLX(); };
float CGraphicFrameData::GetFloatTLX(){ return (float)m_tlx * kTexScale_c1b9e0; }
int main(){
    CGraphicFrameData g; g.m_tlx=256;
    if(g.GetFloatTLX()!=1.0f){std::printf("BAD\n");return 1;}
    std::printf("TLX9E0_OK\n"); return 0;
}