#include "rebuild_abi.h"
#include <cstdio>
struct CTCInventoryBase { char pad00[0x30]; unsigned char m_flag; unsigned char GetCanItemBeQuickAssigned(); };
unsigned char CTCInventoryBase::GetCanItemBeQuickAssigned(){ return m_flag; }
int main(){
    CTCInventoryBase b; b.m_flag=7;
    if(b.GetCanItemBeQuickAssigned()!=7){std::printf("BAD\n");return 1;}
    std::printf("QUICKASSIGN_OK\n"); return 0;
}