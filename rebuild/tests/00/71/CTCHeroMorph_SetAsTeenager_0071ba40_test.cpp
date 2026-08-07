#include "rebuild_abi.h"
#include <cstdio>
struct CTCHeroMorph { char pad3d[0x3d]; unsigned char m_changed; char pad[0x60-0x3e]; unsigned char m_val; void SetAsTeenager(unsigned char v); };
void CTCHeroMorph::SetAsTeenager(unsigned char v){ m_changed=(v!=m_val)?1:0; m_val=v; }
int main(){ CTCHeroMorph o; o.m_val=0; o.m_changed=9;
 o.SetAsTeenager(1); if(o.m_changed!=1||o.m_val!=1){std::printf("B1\n");return 1;}
 o.SetAsTeenager(1); if(o.m_changed!=0){std::printf("B2\n");return 1;}
 std::printf("ST_71ba40_OK\n"); return 0; }