// CMemoryAllocator::ClearPerformanceStats @ 009f26b0
struct CMemoryAllocator;

// stdcall so the callee cleans up its own 4-byte arg (no "add esp,4" after call).
typedef void (__stdcall *ClearFn)(void*);

// The object whose address (0x13caa70) is pushed to both calls.
extern int g_perfStatsObject;

// Two IAT-imported function pointers at [0x143fe0c] and [0x143fe10].
extern ClearFn g_clearFn0;
extern ClearFn g_clearFn1;

void __fastcall ClearPerformanceStats(CMemoryAllocator* self)
{
    (void)self;
    g_clearFn0(&g_perfStatsObject);
    g_clearFn1(&g_perfStatsObject);
}