#include "rebuild_abi.h"
#include <cstdio>
struct CStaticFontBank { char pad00[4]; unsigned int m_maxHeight; float GetFontMaxHeight(); };
float CStaticFontBank::GetFontMaxHeight(){ return (float)m_maxHeight; }
int main(){
    CStaticFontBank b; b.m_maxHeight=20;
    if(b.GetFontMaxHeight()!=20.0f){std::printf("BAD\n");return 1;}
    std::printf("FONTH_OK\n"); return 0;
}