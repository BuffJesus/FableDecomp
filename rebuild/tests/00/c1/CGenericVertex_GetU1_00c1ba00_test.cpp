#include "rebuild_abi.h"
#include <cstdio>
const float kUvScale_c1ba00 = 0.00390625f;
struct CGenericVertex { char pad00[1]; unsigned char m_u1; float GetU1(); };
float CGenericVertex::GetU1(){ return (float)m_u1 * kUvScale_c1ba00; }
int main(){
    CGenericVertex v; v.m_u1=128;
    if(v.GetU1()!=0.5f){std::printf("BAD\n");return 1;}
    std::printf("U1A00_OK\n"); return 0;
}