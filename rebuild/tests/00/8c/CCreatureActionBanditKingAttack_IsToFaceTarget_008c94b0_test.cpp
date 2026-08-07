#include "rebuild_abi.h"
#include <cstdio>
struct CCreatureAction_BanditKingAttack { char pad[0x114]; unsigned char m_v; char IsToFaceTarget(); };
char CCreatureAction_BanditKingAttack::IsToFaceTarget(){ return (char)m_v; }
int main(){ CCreatureAction_BanditKingAttack o; o.m_v=7;
 if(o.IsToFaceTarget()!=7){std::printf("B1\n");return 1;}
 std::printf("FT_8c94b0_OK\n"); return 0; }