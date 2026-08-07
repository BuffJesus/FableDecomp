#include "rebuild_abi.h"
// CMemoryAllocator::ClearPerformanceStats @ 0x0059d9ad
struct CMemoryAllocator { char dummy; void Clear(int n); void ClearPerformanceStats(); };
void CMemoryAllocator::ClearPerformanceStats(){ Clear(0xc); Clear(0xb); }