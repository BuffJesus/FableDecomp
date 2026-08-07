#include "rebuild_abi.h"
#include <cstdio>
static int g_calls=0, g_last=0;
struct CMemoryAllocator { char dummy; void Clear(int n); void ClearPerformanceStats(); };
void CMemoryAllocator::Clear(int n){ g_calls++; g_last=n; }
void CMemoryAllocator::ClearPerformanceStats(){ Clear(0xc); Clear(0xb); }
int main(){ CMemoryAllocator o; o.ClearPerformanceStats();
 if(g_calls!=2||g_last!=0xb){std::printf("B1\n");return 1;}
 std::printf("CP_59d9ad_OK\n"); return 0; }