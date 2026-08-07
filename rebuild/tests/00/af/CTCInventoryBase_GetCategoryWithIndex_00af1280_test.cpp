#include "rebuild_abi.h"
#include <cstdio>
struct CCategory { char data[12]; };
struct CTCInventoryBase { char pad00[0x1e0]; CCategory *m_categories; CCategory *GetCategoryWithIndex(long index); };
CCategory *CTCInventoryBase::GetCategoryWithIndex(long index){ return &m_categories[index]; }
int main(){
    CCategory arr[4]; CTCInventoryBase inv; inv.m_categories=arr;
    if(inv.GetCategoryWithIndex(0)!=&arr[0]){std::printf("B0\n");return 1;}
    if(inv.GetCategoryWithIndex(3)!=&arr[3]){std::printf("B3\n");return 1;}
    std::printf("GETCAT_OK\n"); return 0;
}